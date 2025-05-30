#pragma once

#include "IRequestHandler.h"
#include "codec/c2s/request/Request.h"

class Game;
class RequestHandlerFactory;

class FinishedGameEarlyRequestHandler : public IRequestHandler
{
public:
    explicit FinishedGameEarlyRequestHandler(const RequestHandlerFactory &handlerFactory, Game &game);

    bool isRequestRelevant(const RequestInfo &info) const override;

    RequestResult handleRequest(const RequestInfo &info, const ProtocolRequest &request) const override;

private:
    Game &m_game;

    RequestResult leaveGame(const RequestInfo &info, const LeaveGameRequest &request) const;
};