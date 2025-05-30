#pragma once

#include "IRequestHandler.h"
#include "codec/c2s/request/Request.h"

class RequestHandlerFactory; // Circle dependency

class FinishedGameEarlyRequestHandler : public IRequestHandler
{
public:
    explicit FinishedGameEarlyRequestHandler(const RequestHandlerFactory &handlerFactory);

    bool isRequestRelevant(const RequestInfo &info) const override;

    RequestResult handleRequest(const RequestInfo &info, const ProtocolRequest &request) const override;

private:
    RequestResult leaveGame(const RequestInfo &info, const LeaveGameRequest &request) const;
};