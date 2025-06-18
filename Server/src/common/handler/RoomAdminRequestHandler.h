#pragma once

#include "IRequestHandler.h"
#include "RoomRequestHandler.h"

struct KickPlayerRequest;
struct GetRoomStateRequest;
struct UpdateRoomDataRequest;
struct CloseRoomRequest;
struct StartGameRequest;

class Room;

class RoomAdminRequestHandler : public RoomRequestHandler
{
public:
    explicit RoomAdminRequestHandler(const RequestHandlerFactory& handlerFactory, Room& room);

    std::optional<ErrorStatus> isRequestRelevant(const RequestInfo &info) const override;

    RequestResult handleRequest(const RequestInfo& info, const ProtocolRequest& request) const override;

private:
    RequestResult startGame(const RequestInfo &info, const StartGameRequest &request) const;
    RequestResult closeRoom(const RequestInfo &info, const CloseRoomRequest &request) const;

    RequestResult updateRoomData(const RequestInfo &info, const UpdateRoomDataRequest &request) const;

    RequestResult kick(const RequestInfo &info, const KickPlayerRequest &request) const;
    std::optional<RequestResult> checkRoomData(const RequestInfo &info, const RoomData &data) const;
};
