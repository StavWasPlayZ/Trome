#pragma once

#include "IRequestHandler.h"
#include "codec/c2s/request/Request.h"
#include "RequestHandlerFactory.h"

class Game;
class RequestHandlerFactory; // Circle dependency

class FinishedGameEarlyRequestHandler : public IRequestHandler
{
public:
    explicit FinishedGameEarlyRequestHandler(const RequestHandlerFactory &handlerFactory, Game &game);

    bool isRequestRelevant(const RequestInfo &info) const override;

    RequestResult handleRequest(const RequestInfo &info, const ProtocolRequest &request) const override;

private:
    Game &game;

    RequestResult leaveGame(const RequestInfo &info, const LeaveGameRequest &request) const;
};