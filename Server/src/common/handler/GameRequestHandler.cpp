#include "GameRequestHandler.h"

#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "FinishedGameEarlyRequestHandler.h"
#include "codec/s2c/response/JsonResponsePacketSerializer.h"
#include "codec/s2c/response/Response.h"
#include "handler/RequestHandlerFactory.h"
#include "infrastructure/Client.h"

GameRequestHandler::GameRequestHandler(const RequestHandlerFactory &handlerFactory, Game& game) :
    IRequestHandler(handlerFactory),
    m_game(game)
{}

bool GameRequestHandler::isRequestRelevant(const RequestInfo &info) const
{
    switch (info.id)
    {
    case RequestCode::SUBMIT_ANSWER:
    case RequestCode::LEAVE_GAME:
    case RequestCode::GET_QUESTION:
    case RequestCode::GET_GAME_RESULT:
        return true;

    default: return false;
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

    Room& room = this->m_game.getRoom();

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

    const std::optional<UserQuestion> newQuestion = this->m_game.generateNewQuestionForUser(user, didFail);
    const int playersFinished = this->m_game.getPlayersFinished();

    if (this->m_game.isGameComplete())
    {
        handleLastPlayerFinished(info);

        return RequestResult(
            new SubmitAnswerResponse(newQuestion, userData.points, true, playersFinished),
            getMenuRequestHandlerFor(user)
        );
    }


    // If there is no new question available, we've finished early.
    if (!newQuestion.has_value())
    {
        dispatchNotification(
            PlayerFinishedNotification(), 
            room.getAllUsers(),
            &user
        );

        return RequestResult(
            new SubmitAnswerResponse(std::nullopt, userData.points, false, playersFinished),
            new FinishedGameEarlyRequestHandler(m_handlerFactory, m_game)
        );
    }

    return RequestResult(new SubmitAnswerResponse(newQuestion, userData.points, false, playersFinished));
}

RequestResult GameRequestHandler::leaveGame(const RequestInfo &info, const LeaveGameRequest &) const
{
    Room& room = m_game.getRoom();
    const LoggedUser& user = getUserByInfo(info);

    if (room.getAdmin() == user)
    {
        this->m_handlerFactory.getRoomManager().deleteRoom(room);
    }
    else
    {
        room.removeUser(getUserByInfo(info));
    }

    return RequestResult(
        new LeaveGameResponse(),
        new MenuRequestHandler(m_handlerFactory)
    );
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

    const int playersFinished = this->m_game.getPlayersFinished();

    if (userData.didYetStart())
    {
        const UserQuestion question = m_game.setFirstQuestionForUser(user);
        return RequestResult(new GetQuestionResponse(question, 0, false, playersFinished));
    }

    // Getting here means the user has either skipped the question or that the time has passed.
    // Either of which will prompt the failure of the current round.
    const std::optional<UserQuestion> newQuestion = m_game.generateNewQuestionForUser(user, true);

    if (this->m_game.isGameComplete())
    {
        handleLastPlayerFinished(info);

        return RequestResult(
            new GetQuestionResponse(newQuestion, userData.points, true, playersFinished),
            getMenuRequestHandlerFor(user)
        );
    }

    return RequestResult(new GetQuestionResponse(newQuestion, userData.points, false, playersFinished));
}

RequestResult GameRequestHandler::getGameResults(const RequestInfo &info, const GetGameResultRequest &) const
{
    return RequestResult(new ErrorResponse(ErrorStatus::SERVER_UNIMPLEMENTED, info.id));
}

void GameRequestHandler::handleLastPlayerFinished(const RequestInfo &info) const
{
    const LoggedUser &user = getUserByInfo(info);

    setRequestHandlers(
        [this](const LoggedUser *player) {
            return getMenuRequestHandlerFor(*player);
        },

         m_game.getRoom().getAllUsers(),
        // TODO: Provide game results
        GameEndedNotification(m_game),
        &user
    );
}

IRequestHandler *GameRequestHandler::getMenuRequestHandlerFor(const LoggedUser &user) const
{
    Room &room = m_game.getRoom();

    return user == room.getAdmin()
        ? static_cast<IRequestHandler *>(new RoomAdminRequestHandler(this->m_handlerFactory, room))
        : static_cast<IRequestHandler *>(new RoomMemberRequestHandler(this->m_handlerFactory, room));
}
