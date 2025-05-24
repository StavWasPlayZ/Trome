#include "RoomAdminRequestHandler.h"

#include "codec/c2s/request/Request.h"
#include "codec/s2c/response/ErrorResponse.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(const RequestHandlerFactory &handlerFactory, Room& room) :
    IRequestHandler(handlerFactory),
    m_room(room)
{}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo &info) const
{
    switch (info.id)
    {
	case RequestCode::JOIN_ROOM:
    case RequestCode::GET_PLAYERS_IN_ROOM:
    case RequestCode::CREATE_ROOM:
    case RequestCode::GET_ROOMS:
    case RequestCode::GET_HIGH_SCORES:
    case RequestCode::GET_PERSONAL_STATISTICS:
    case RequestCode::LOGOUT:
        return true;

    default: return false;
    }
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo &info, const ProtocolRequest &request) const
{
    switch (info.id)
    {
    case RequestCode::START_GAME:
        return startGame(info, request);
    case RequestCode::CLOSE_ROOM:
        return closeRoom(info, request);

    case RequestCode::GET_ROOM_STATE:
        return getRoomState(info, request);

    default: throw std::invalid_argument("Unknown request ID");
    }
}

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo &info, const ProtocolRequest &) const
{
    return RequestResult(
        new ErrorResponse(ErrorStatus::SERVER_UNIMPLEMENTED, info.id),
        new RoomAdminRequestHandler(*this)
    );
}

RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo &info, const ProtocolRequest &) const
{
    return RequestResult(
        new ErrorResponse(ErrorStatus::SERVER_UNIMPLEMENTED, info.id),
        new RoomAdminRequestHandler(*this)
    );
}

RequestResult RoomAdminRequestHandler::setRoomState(const RequestInfo &info, const ProtocolRequest &) const
{
    return RequestResult(
        new ErrorResponse(ErrorStatus::SERVER_UNIMPLEMENTED, info.id),
        new RoomAdminRequestHandler(*this)
    );
}

RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo &info, const ProtocolRequest &) const
{
    return RequestResult(new GetRoomStateResponse(m_room), new RoomAdminRequestHandler(*this));
}
