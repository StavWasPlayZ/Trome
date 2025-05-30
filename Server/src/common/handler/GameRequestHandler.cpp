#include "GameRequestHandler.h"

#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
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
    Room& room = this->m_game.getRoom();

    // Check if the question was submitted in time (+1sec for server delay).
    // (The user is the one responsible for fetching a new question for that matter)
    if (m_game.getDataOf(user).getRoundTime() > room.getData().getTimePerQuestionMs())
    {
        return RequestResult(
            new ErrorResponse(ErrorStatus::QUESTION_OUTDATED, info.id)
        );
    }

    const UserQuestion question = this->m_game.getQuestionForUser(user).value();


    // If the returned answer is 0 unrotated, it must be correct.
    // This is because the first answer is always the correct one.
    const bool didFail = request.answer - question.rotation == 0;

    const std::optional<UserQuestion> newQuestion = this->m_game.generateNewQuestionForUser(user, didFail);

    if (this->m_game.isGameComplete())
    {
        setRequestHandlers(
            [this, &room](const LoggedUser* player) {
                return *player == room.getAdmin()
                    ? static_cast<const IRequestHandler*>(new RoomAdminRequestHandler(this->m_handlerFactory, room))
                    : static_cast<const IRequestHandler*>(new RoomMemberRequestHandler(this->m_handlerFactory, room))
                ;
            },

            room.getAllUsers(),
            //TODO: Provide game results
            GameEndedNotification(),
            &user
        );

        return RequestResult(
            new SubmitAnswerResponse(newQuestion, true),

            user == room.getAdmin()
                ? static_cast<const IRequestHandler*>(new RoomAdminRequestHandler(m_handlerFactory, room))
                : static_cast<const IRequestHandler*>(new RoomMemberRequestHandler(m_handlerFactory, room))
        );
    }

    return RequestResult(
        new SubmitAnswerResponse(newQuestion, false)
        //TODO: If newQuestion is empty (finished early), return a waiting handler.
    );
}

RequestResult GameRequestHandler::leaveGame(const RequestInfo &info, const LeaveGameRequest &) const
{
    m_game.getRoom().removeUser(getUserByInfo(info));

    return RequestResult(
        new LeaveRoomResponse(),
        new MenuRequestHandler(m_handlerFactory)
    );
}

RequestResult GameRequestHandler::getQuestion(const RequestInfo &info, const GetQuestionRequest &) const
{
    //NOTE: A known vulnerability here is that the user can just never send this request and deadlock everyone
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

    const LoggedUser& user = getUserByInfo(info);

    // User has already finished; Just return nothing
    if (m_game.getDataOf(user).isFinished)
    {
        return RequestResult(new GetQuestionResponse(std::nullopt));
    }

    // Getting here means the user has either skipped the question or that the time has passed.
    // Either of which will prompt  the failure of the current round.
    const std::optional<UserQuestion> newQuestion = m_game.generateNewQuestionForUser(user, true);

    return RequestResult(new GetQuestionResponse(newQuestion));
}

RequestResult GameRequestHandler::getGameResults(const RequestInfo &info, const GetGameResultRequest &) const
{
    return RequestResult(
        new ErrorResponse(ErrorStatus::SERVER_UNIMPLEMENTED, info.id)
    );
}
