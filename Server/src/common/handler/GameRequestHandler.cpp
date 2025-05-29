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

RequestResult GameRequestHandler::submitAnswer(const RequestInfo &info, const SubmitAnswerRequest &) const
{
    return RequestResult(
        new ErrorResponse(ErrorStatus::SERVER_UNIMPLEMENTED, info.id)
    );
}

RequestResult GameRequestHandler::leaveGame(const RequestInfo &info, const LeaveGameRequest &) const
{
    return RequestResult(
        new ErrorResponse(ErrorStatus::SERVER_UNIMPLEMENTED, info.id)
    );
}

RequestResult GameRequestHandler::getQuestion(const RequestInfo &info, const GetQuestionRequest &) const
{
    const UserQuestion question = m_game.getQuestionForUser(getUserByInfo(info));

    return RequestResult(
        new GetQuestionResponse(question.question, question.rotation)
    );
}

RequestResult GameRequestHandler::getGameResults(const RequestInfo &info, const GetGameResultRequest &) const
{
    return RequestResult(
        new ErrorResponse(ErrorStatus::SERVER_UNIMPLEMENTED, info.id)
    );
}
