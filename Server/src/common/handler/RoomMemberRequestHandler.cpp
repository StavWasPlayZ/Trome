#include "RoomMemberRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "codec/c2s/request/Request.h"
#include "codec/s2c/notification/Notification.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(const RequestHandlerFactory &handlerFactory, Room &room)
    : IRequestHandler(handlerFactory), m_room(room)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo &info) const
{
    switch (info.id)
    {
    case RequestCode::LEAVE_ROOM:
    case RequestCode::GET_ROOM_STATE:
        return true;

    default:
        return false;
    }
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo &info, const ProtocolRequest &request) const
{
    switch (info.id)
    {
    case RequestCode::LEAVE_ROOM:
        return leaveRoom(info, static_cast<const LeaveRoomRequest &>(request));
    case RequestCode::GET_ROOM_STATE:
        return getRoomState(info, static_cast<const GetRoomStateRequest &>(request));

    default:
        throw std::invalid_argument("Unknown request ID");
    }
}

RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo &info, const LeaveRoomRequest &) const
{
    LoggedUser& user = getUserByInfo(info);
    m_room.removeUser(user);

    return RequestResult(
        new LeaveRoomResponse(),
        new MenuRequestHandler(this->m_handlerFactory)
    );
}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo &, const GetRoomStateRequest &) const
{
    return RequestResult(new GetRoomStateResponse(m_room));
}
