#pragma once

#include "IRequestHandler.h"
#include "infrastructure/Room.h"
#include "manager/RoomManager.h"

class RoomMemberRequestHandler : public IRequestHandler
{
public:
    RoomMemberRequestHandler(const RequestHandlerFactory &handlerFactory, const Room &room, const RoomManager manager);

    bool isRequestRelevant(const RequestInfo &info) const;
    RequestResult handleRequest(const RequestInfo &info, const ProtocolRequest &request) const;
private:
    Room m_room;
    RoomManager m_roomManager;

    RequestResult leaveRoom(const RequestInfo &context, const LeaveRoomResponse &request) const;
    RequestResult getRoomState(const RequestInfo &context, const GetRoomStateResponse &request) const;
};