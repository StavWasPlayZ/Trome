#include "GameRequestHandler.h"

#include "FinishedGameEarlyRequestHandler.h"
#include "codec/s2c/response/JsonResponsePacketSerializer.h"
#include "codec/s2c/response/Response.h"
#include "handler/RequestHandlerFactory.h"
#include "infrastructure/Client.h"
#include "manager/GameManager.h"

GameRequestHandler::GameRequestHandler(const RequestHandlerFactory &handlerFactory, Game& game) :
    IRequestHandler(handlerFactory),
    m_game(game)
{}

std::optional<ErrorStatus> GameRequestHandler::isRequestRelevant(const RequestInfo &info) const
{
    if (info.receivalTime < m_game.getStartTime())
    {
        return ErrorStatus::ANSWER_TOO_EARLY;
    }

    switch (info.id)
    {
    case RequestCode::SUBMIT_ANSWER:
    case RequestCode::LEAVE_GAME:
    case RequestCode::GET_QUESTION:
    case RequestCode::GET_GAME_RESULT:
        return std::nullopt;

    default: return ErrorStatus::ILLEGAL_REQUEST;
    }
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo &info, const ProtocolRequest &request) const
{
    switch (info.id)
    {
    case RequestCode::SUBMIT_ANSWER:
        return submitAnswer(info, static_cast<const SubmitAnswerRequest &>(request));
    case RequestCode::LEAVE_GAME:
        return leaveGame(info, static_cast<const LeaveGameRequest &>(request));
    case RequestCode::GET_QUESTION:
        return getQuestion(info, static_cast<const GetQuestionRequest &>(request));

    case RequestCode::GET_GAME_RESULT:
        return getGameResults(info, static_cast<const GetGameResultRequest &>(request));

    default:
        throw std::runtime_error("Unexpected request ID");
    }
}

RequestResult GameRequestHandler::submitAnswer(const RequestInfo &info, const SubmitAnswerRequest &request) const
{
    const LoggedUser& user = getUserByInfo(info);
    const GameData& userData = m_game.getDataOf(user);

    if (userData.didYetStart())
    {
        return RequestResult(new ErrorResponse(ErrorStatus::ILLEGAL_REQUEST, info.id));
    }

    Room &room = this->m_game.getRoom();

    // Check if the question was submitted in time (+1sec for server delay).
    // (The user is the one responsible for fetching a new question for that matter)
    if (userData.getRoundTime() > room.getData().getTimePerQuestionMs())
    {
        return RequestResult(
            new ErrorResponse(ErrorStatus::QUESTION_OUTDATED, info.id)
        );
    }


    const UserQuestion question = this->m_game.getQuestionForUser(user).value();

    // If the returned answer is 0 unrotated, it must be correct.
    // This is because the first answer is always the correct one.
    const bool didFail = ((4 - request.answer) % 4) - question.rotation != 0;


    const QuestionRollResult rollResult = rollNewUserQuestion(info, didFail);


    switch (rollResult.rollType)
    {
    case QuestionRollType::ROLLED:
        return RequestResult(new SubmitAnswerResponse(rollResult.newQuestion, userData.points));

    case QuestionRollType::FINISHED:
        return RequestResult(
            new SubmitAnswerResponse(std::nullopt, userData.points),
            new FinishedGameEarlyRequestHandler(this->m_handlerFactory, this->m_game)
        );

    case QuestionRollType::FINISHED_LAST:
        return finalizeLastPlayerFinished(info, new SubmitAnswerResponse(
            rollResult.newQuestion,
            userData.points,
            rollResult.results
        ));

    // ReSharper disable once CppDFAUnreachableCode
    default:
        throw std::runtime_error("Invalid roll state");
    }
}

RequestResult GameRequestHandler::leaveGame(const RequestInfo &info, const LeaveGameRequest &) const
{
    Room &room = m_game.getRoom();
    const LoggedUser &user = getUserByInfo(info);

    if (room.getAdmin() == user)
    {
        this->m_handlerFactory.getRoomManager().deleteRoom(room);
    }
    else
    {
        room.removeUser(getUserByInfo(info));

        if (this->m_game.isGameComplete())
        {
            handleLastPlayerFinished(info);
            this->m_handlerFactory.getGameManager().deleteGame(m_game);
        }
    }

    return RequestResult(new LeaveGameResponse(), new MenuRequestHandler(m_handlerFactory));
}

QuestionRollResult GameRequestHandler::rollNewUserQuestion(const RequestInfo &info, const bool didFail) const
{
    const LoggedUser &user = getUserByInfo(info);
    const Room &room = this->m_game.getRoom();

    const std::optional<UserQuestion> newQuestion = this->m_game.generateNewQuestionForUser(user, didFail);

    if (this->m_game.isGameComplete())
    {
        const std::vector<PlayerResult> results = handleLastPlayerFinished(info);

        return QuestionRollResult(QuestionRollType::FINISHED_LAST, std::nullopt, results);
    }

    // If there is no new question available, we've finished early.
    if (!newQuestion.has_value())
    {
        dispatchNotification(
            PlayerFinishedNotification(),
            room.getAllUsers(),
            &user
        );

        return QuestionRollResult(QuestionRollType::FINISHED, newQuestion);
    }

    return QuestionRollResult(QuestionRollType::ROLLED, newQuestion);
}

RequestResult GameRequestHandler::getQuestion(const RequestInfo &info, const GetQuestionRequest &) const
{
    const LoggedUser& user = getUserByInfo(info);
    const GameData& userData = m_game.getDataOf(user);

    // User has already finished
    if (userData.isFinished)
    {
        return RequestResult(new ErrorResponse(ErrorStatus::ALREADY_FINISHED, info.id));
    }

    //NOTE: A vulnerability here is that the user can just never send this request and deadlock everyone
    // in the room.
    //
    // The fix is to make a server-bound timer for every player that will invoke a version of this method on timeout,
    // and notify the clients of the new question.
    //
    // However that is slightly too complex to implement, since not only do we need to time our function
    // but also require a cancellation token in hand for if the user has actually answered.
    //
    // Of course, a simple std::thread per player per question could work but will surely bombard the server
    // to oblivion.
    //
    // A more realistic solution would probably be to use a single-threaded round timeouts handler that does
    // all the above, much like in JS.
    //
    // That is to say, **Asio (preferable)** or *LibUV (uv-cpp)*.
    //
    // Another option is a thread per player with the same concept in mind, but I firmly believe that is just wasteful.
    //
    // TODO: (probably never) fix

    if (userData.didYetStart())
    {
        const UserQuestion question = m_game.setFirstQuestionForUser(user);
        return RequestResult(new GetQuestionResponse(question, 0));
    }

    // Getting here means the user has either skipped the question or that the time has passed.
    // Either of which will prompt the failure of the current round.
    const QuestionRollResult rollResult = rollNewUserQuestion(info, true);


    switch (rollResult.rollType)
    {
    case QuestionRollType::ROLLED:
        return RequestResult(new GetQuestionResponse(rollResult.newQuestion, userData.points));

    case QuestionRollType::FINISHED:
        return RequestResult(
            new GetQuestionResponse(std::nullopt, userData.points),
            new FinishedGameEarlyRequestHandler(this->m_handlerFactory, this->m_game)
        );

    case QuestionRollType::FINISHED_LAST:
        return finalizeLastPlayerFinished(info, new GetQuestionResponse(
            rollResult.newQuestion,
            userData.points,
            rollResult.results
        ));

    // ReSharper disable once CppDFAUnreachableCode
    default:
        throw std::runtime_error("Invalid roll state");
    }
}

RequestResult GameRequestHandler::getGameResults(const RequestInfo &, const GetGameResultRequest &) const
{
    return RequestResult(new GetGameResultResponse(m_game.getResults()));
}

RequestResult GameRequestHandler::finalizeLastPlayerFinished(const RequestInfo &info,
                                                             const QuestionResponse *const response) const
{
    Room &room = this->m_game.getRoom();
    this->m_handlerFactory.getGameManager().deleteGame(m_game);

    return RequestResult(
        response,
        this->m_handlerFactory.createRoomRequestHandler(
            getUserByInfo(info),
            room
        )
    );
}

std::vector<PlayerResult> GameRequestHandler::handleLastPlayerFinished(const RequestInfo &info) const
{
    const std::vector<PlayerResult> results = m_game.getResults();

    setRequestHandlers(
        [this](const LoggedUser *player) {
            return this->m_handlerFactory.createRoomRequestHandler(*player, this->m_game.getRoom());
        },

        m_game.getRoom().getAllUsers(),
        GameEndedNotification(results),
        &getUserByInfo(info)
    );

    return results;
}

QuestionRollResult::QuestionRollResult(const QuestionRollType rollType, const std::optional<UserQuestion> &newQuestion,
                                       const std::optional<std::vector<PlayerResult>>& results) :
    rollType(rollType),
    newQuestion(newQuestion),
    results(results)
{}
