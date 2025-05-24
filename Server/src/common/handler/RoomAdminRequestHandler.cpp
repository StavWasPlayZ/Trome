#include "RoomAdminRequestHandler.h"

#include "RequestHandlerFactory.h"
#include "codec/c2s/request/Request.h"
#include "codec/s2c/notification/Notification.h"
#include "codec/s2c/response/ErrorResponse.h"
#include "manager/RoomManager.h"

#include <algorithm>

RoomAdminRequestHandler::RoomAdminRequestHandler(const RequestHandlerFactory &handlerFactory, Room& room) :
    IRequestHandler(handlerFactory),
    m_room(room)
{}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo &info) const
{
    switch (info.id)
    {
	case RequestCode::START_GAME:
    case RequestCode::CLOSE_ROOM:
    case RequestCode::UPDATE_ROOM_DATA:
    case RequestCode::GET_ROOM_STATE:
        return true;

    default: return false;
    }
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo &info, const ProtocolRequest &request) const
{
    switch (info.id)
    {
    case RequestCode::START_GAME:
        return startGame(info, static_cast<const StartGameRequest &>(request));
    case RequestCode::CLOSE_ROOM:
        return closeRoom(info, static_cast<const CloseRoomRequest &>(request));
    case RequestCode::UPDATE_ROOM_DATA:
        return updateRoomData(info, static_cast<const UpdateRoomDataRequest &>(request));

    case RequestCode::GET_ROOM_STATE:
        return getRoomState(info, static_cast<const GetRoomsRequest &>(request));

    default: throw std::invalid_argument("Unknown request ID");
    }
}

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo &info, const StartGameRequest &) const
{
    //TODO: Implement

    return RequestResult(
        new ErrorResponse(ErrorStatus::SERVER_UNIMPLEMENTED, info.id),
        new RoomAdminRequestHandler(*this)
    );
}

RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo &info, const CloseRoomRequest &) const
{
    RoomManager &rManager = m_handlerFactory.getRoomManager();

    const LoggedUser &user = getUserByInfo(info);
    const std::vector<LoggedUser*>& players = m_room.getAllUsers();

    std::vector<LoggedUser*> usersNoAdmin;

    std::ranges::copy_if(
        players, std::back_inserter(usersNoAdmin),
        [&user](const LoggedUser* player) {
            return *player != user;
        }
    );

    // Release all players from the RoomMemberRequestHandler state
    for (const LoggedUser* player : usersNoAdmin)
    {
        Client& client = player->getClient();
        client.lockRequestHandler();

        delete client.getRequestHandler();
        client.setRequestHandler(new MenuRequestHandler(m_handlerFactory));

        client.releaseRequestHandler();
    }

    rManager.deleteRoom(m_room.getId());

    return RequestResult(
        new CloseRoomResponse(),
        new MenuRequestHandler(m_handlerFactory),

        new NotificationPayload(
            new RoomClosedNotification(),
            usersNoAdmin
        )
    );
}

RequestResult RoomAdminRequestHandler::updateRoomData(const RequestInfo &info, const UpdateRoomDataRequest &) const
{
    return RequestResult(
        new ErrorResponse(ErrorStatus::SERVER_UNIMPLEMENTED, info.id),
        new RoomAdminRequestHandler(*this)
    );
}

RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo &, const GetRoomsRequest &) const
{
    return RequestResult(new GetRoomStateResponse(m_room), new RoomAdminRequestHandler(*this));
}
