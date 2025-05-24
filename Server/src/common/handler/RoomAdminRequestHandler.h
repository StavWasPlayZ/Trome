#pragma once

#include "IRequestHandler.h"

struct GetRoomsRequest;
struct UpdateRoomDataRequest;
struct CloseRoomRequest;
struct StartGameRequest;

class Room;

class RoomAdminRequestHandler : public IRequestHandler
{
public:
    explicit RoomAdminRequestHandler(const RequestHandlerFactory& handlerFactory, Room& room);

    bool isRequestRelevant(const RequestInfo &info) const override;

    RequestResult handleRequest(const RequestInfo& info, const ProtocolRequest& request) const override;

private:
    RequestResult startGame(const RequestInfo & info, const StartGameRequest & request) const;
    RequestResult closeRoom(const RequestInfo & info, const CloseRoomRequest & request) const;

    RequestResult updateRoomData(const RequestInfo & info, const UpdateRoomDataRequest & request) const;

    [[deprecated(
        "The Noftifications system has been set in place to allow for automatic, non-polling updates of any "
        "room state changes."
        " This method is therefore useless and should not be used."
    )]]
    RequestResult getRoomState(const RequestInfo& info, const GetRoomsRequest& request) const;

    Room& m_room;
};
