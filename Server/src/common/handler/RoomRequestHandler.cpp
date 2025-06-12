#include "RoomRequestHandler.h"

#include "codec/c2s/request/Request.h"
#include "codec/s2c/response/ErrorResponse.h"
#include "codec/s2c/response/Response.h"

RoomRequestHandler::RoomRequestHandler(const RequestHandlerFactory &handlerFactory, Room &room) :
    IRequestHandler(handlerFactory),
    m_room(room)
{}

std::optional<ErrorStatus> RoomRequestHandler::isRequestRelevant(const RequestInfo &info) const
{
    switch (info.id)
    {
    case RequestCode::GET_PLAYERS_IN_ROOM:
    case RequestCode::GET_ROOM_STATE:
        return std::nullopt;

    default:
        return ErrorStatus::ILLEGAL_REQUEST;
    }
}

RequestResult RoomRequestHandler::handleRequest(const RequestInfo &info, const ProtocolRequest &request) const
{
    switch (info.id)
    {
    case RequestCode::GET_PLAYERS_IN_ROOM:
        return getPlayersInRoom(info, static_cast<const GetPlayersInRoomRequest &>(request));

    case RequestCode::GET_ROOM_STATE:
        return getRoomState(info, static_cast<const GetRoomStateRequest &>(request));

    default:
        throw std::invalid_argument("Unknown request ID");
    }
}

RequestResult RoomRequestHandler::getPlayersInRoom(const RequestInfo &, const GetPlayersInRoomRequest &) const
{
    return RequestResult(new GetPlayersInRoomResponse(this->m_room.getAllUsers()));
}

RequestResult RoomRequestHandler::getRoomState(const RequestInfo &, const GetRoomStateRequest &) const
{
    return RequestResult(new GetRoomStateResponse(m_room));
}