#pragma once

#include "IRequestHandler.h"
#include "codec/c2s/request/Request.h"

class RequestHandlerFactory;  // Circle dependency

class GameRequestHandler : public IRequestHandler
{
public:
    explicit GameRequestHandler(const RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& info) const override;

    RequestResult handleRequest(const RequestInfo& info, const ProtocolRequest& request) const override;

private:
    RequestResult submitAnswer(const RequestInfo &info, const SubmitAnswerRequest &request) const;
    RequestResult leaveGame(const RequestInfo &info, const LeaveGameRequest &request) const;

    [[deprecated(
        "SubmitAnswerResponse already returns the next question."
    )]]
    RequestResult getQuestion(const RequestInfo &info, const GetQuestionRequest &request) const;

    [[deprecated(
        "The Noftifications system has been set in place to allow for automatic, non-polling updates of any "
        "room state changes."
        " This method is therefore useless and should not be used."
    )]]
    RequestResult getGameResults(const RequestInfo &info, const GetGameResultRequest &request) const;
};
