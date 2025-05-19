#include "RoomMemberRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "codec/c2s/Request.h"

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
        return leaveRoom(info, request);
    case RequestCode::GET_ROOM_STATE:
        return getRoomState(info, request);

    default:
        throw std::invalid_argument("Unknown request ID");
    }
}

RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo &info, const ProtocolRequest &request) const
{
    const LeaveRoomRequest &req = static_cast<const LeaveRoomRequest &>(request);

    m_room.removeUser(getUserByInfo(info));

    return RequestResult(
            JsonResponsePacketSerializer::serializeResponse(
                LeaveRoomResponse(GenericResponseStatus::SUCCESS)
        ),

        new MenuRequestHandler(this->m_handlerFactory));
}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo &info, const ProtocolRequest &request) const
{
    const GetRoomStateRequest &req = static_cast<const GetRoomStateRequest &>(request);

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(
            GetRoomStateResponse(GenericResponseStatus::SUCCESS, m_room)
        ),

        new RoomMemberRequestHandler(*this)
    );
}
