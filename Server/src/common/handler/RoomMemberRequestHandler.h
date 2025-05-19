#pragma once

#include "IRequestHandler.h"
#include "infrastructure/Room.h"
#include "manager/RoomManager.h"

class RoomMemberRequestHandler : public IRequestHandler
{
public:
    RoomMemberRequestHandler(const RequestHandlerFactory &handlerFactory, Room &room);

    bool isRequestRelevant(const RequestInfo &info) const override;
    RequestResult handleRequest(const RequestInfo &info, const ProtocolRequest &request) const override;
private:
    Room& m_room;

    RequestResult leaveRoom(const RequestInfo &info, const ProtocolRequest &request) const;
    RequestResult getRoomState(const RequestInfo &info, const ProtocolRequest &request) const;
};