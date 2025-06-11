#pragma once

#include "IRequestHandler.h"
#include "RoomRequestHandler.h"

struct GetRoomStateRequest;
struct LeaveRoomRequest;

class Room;

class RoomMemberRequestHandler : public RoomRequestHandler
{
public:
    RoomMemberRequestHandler(const RequestHandlerFactory &handlerFactory, Room &room);

    bool isRequestRelevant(const RequestInfo &info) const override;
    RequestResult handleRequest(const RequestInfo &info, const ProtocolRequest &request) const override;

private:
    RequestResult leaveRoom(const RequestInfo &info, const LeaveRoomRequest &request) const;
};
