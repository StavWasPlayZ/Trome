#pragma once

#include "IRequestHandler.h"

class MenuRequestHandler : public IRequestHandler
{
public:
    explicit MenuRequestHandler(const RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo &info) const override;

    RequestResult handleRequest(const RequestInfo& info, const ProtocolRequest& request) const override;

private:
    RequestResult joinRoom(const RequestInfo & info, const ProtocolRequest & request) const;
    RequestResult getPlayersInRoom(const RequestInfo & info, const ProtocolRequest & request) const;
    RequestResult createRoom(const RequestInfo& info, const ProtocolRequest& request) const;
    RequestResult getRooms(const RequestInfo& info, const ProtocolRequest& request) const;
    RequestResult getHighScores(const RequestInfo& info, const ProtocolRequest& request) const;
    RequestResult getPersonalStatistics(const RequestInfo& info, const ProtocolRequest& request) const;
    RequestResult logout(const RequestInfo& info, const ProtocolRequest& request) const;
};
