#include "GameRequestHandler.h"

#include "codec/s2c/response/Response.h"
#include "codec/s2c/response/JsonResponsePacketSerializer.h"
#include "handler/RequestHandlerFactory.h"


GameRequestHandler::GameRequestHandler(Game& game, const RequestHandlerFactory &handlerFactory) :
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
    const UserQuestion question = m_game.getQuestionForUser(user);

    //TODO: Check if the question was submitted in time (+1sec for server delay).
    // If so, refuse to answer with an error response.
    // The user is the one responsible for fetching a new question for that matter, under the now-not-deprecated
    // GetQuestionRequest (or a new request ig).


    // If the returned answer is 0 unrotated, it must be correct.
    // This is because the first answer is always the correct one.
    const bool didFail = request.answer - question.rotation == 0;

    std::optional<UserQuestion> newQuestion = std::nullopt;
    // ReSharper disable once CppTooWideScope
    const bool didGenerate = m_game.generateNewQuestionForUser(user, didFail);

    if (didGenerate)
    {
        newQuestion.emplace(m_game.getQuestionForUser(user));
    }

    return RequestResult(
        new SubmitAnswerResponse(newQuestion)
        //TODO: If did not generate (finished early), return a waiting handler or something.
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
    return RequestResult(
        new GetQuestionResponse(m_game.getQuestionForUser(getUserByInfo(info)))
    );
}

RequestResult GameRequestHandler::getGameResults(const RequestInfo &info, const GetGameResultRequest &) const
{
    return RequestResult(
        new ErrorResponse(ErrorStatus::SERVER_UNIMPLEMENTED, info.id)
    );
}
