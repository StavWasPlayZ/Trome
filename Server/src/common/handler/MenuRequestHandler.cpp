#include "MenuRequestHandler.h"

#include "RequestHandlerFactory.h"
#include "RoomMemberRequestHandler.h"
#include "codec/c2s/request/Request.h"

MenuRequestHandler::MenuRequestHandler(const RequestHandlerFactory &handlerFactory) : IRequestHandler(handlerFactory)
{}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo &info) const
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

RequestResult MenuRequestHandler::handleRequest(const RequestInfo &info, const ProtocolRequest &request) const
{
    switch (info.id)
    {
    case RequestCode::JOIN_ROOM: return joinRoom(info, request);
    case RequestCode::GET_PLAYERS_IN_ROOM: return getPlayersInRoom(info, request);
    case RequestCode::CREATE_ROOM: return createRoom(info, request);
    case RequestCode::GET_ROOMS: return getRooms(info, request);
    case RequestCode::GET_HIGH_SCORES: return getHighScores(info, request);
    case RequestCode::GET_PERSONAL_STATISTICS: return getPersonalStatistics(info, request);
    case RequestCode::LOGOUT: return logout(info, request);

    default: throw std::invalid_argument("Unknown request ID");
    }
}

RequestResult MenuRequestHandler::joinRoom(const RequestInfo &info, const ProtocolRequest &request) const
{
    const JoinRoomRequest &req = static_cast<const JoinRoomRequest &>(request);
    RoomManager &rManager = m_handlerFactory.getRoomManager();

    const std::optional<Room*> room = rManager.getRoom(req.roomID);

    if (!room)
    {
        return RequestResult(
            new ErrorResponse(ErrorStatus::ERROR_UNKNOWN_RESOURCE, info.id),
            new MenuRequestHandler(*this)
        );
    }

    room.value()->addUser(getUserByInfo(info));

    return RequestResult(
        new JoinRoomResponse(*room.value(), room.value()->getAllUsers()),
        new RoomMemberRequestHandler(this->m_handlerFactory, *room.value())
    );
}

RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo &info, const ProtocolRequest &request) const
{
    const GetPlayersInRoomRequest &req = static_cast<const GetPlayersInRoomRequest &>(request);
    RoomManager &rManager = m_handlerFactory.getRoomManager();

    const std::optional<Room*> room = rManager.getRoom(req.roomID);

    if (!room)
    {
        return RequestResult(
            new ErrorResponse(ErrorStatus::ERROR_UNKNOWN_RESOURCE, info.id),
            new MenuRequestHandler(*this)
        );
    }

    return RequestResult(
        new GetPlayersInRoomResponse(room.value()->getAllUsers()),
        new MenuRequestHandler(*this)
    );
}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& info, const ProtocolRequest&) const
{
    RoomManager &rManager = m_handlerFactory.getRoomManager();

    const Room& room = rManager.createRoom(getUserByInfo(info), RoomData::ofDefaults());

    return RequestResult(
        new CreateRoomResponse(room.getId(), room.getData()),

        // TODO : make it RoomAdminRequestHandler when its implemented
        new MenuRequestHandler(*this)
    );
}

RequestResult MenuRequestHandler::getRooms(const RequestInfo &, const ProtocolRequest &) const
{
    RoomManager &rManager = m_handlerFactory.getRoomManager();

    return RequestResult(
        new GetRoomsResponse(rManager.getRooms()),
        new MenuRequestHandler(*this)
    );
}

RequestResult MenuRequestHandler::getHighScores(const RequestInfo &, const ProtocolRequest &) const
{
    const StatisticsManager &sManager = m_handlerFactory.getStatisticsManager();

    return RequestResult(
        new GetHighScoresResponse(sManager.getHighScores()),
        new MenuRequestHandler(*this)
    );
}

RequestResult MenuRequestHandler::getPersonalStatistics(const RequestInfo& info, const ProtocolRequest &) const
{
    const StatisticsManager &sManager = m_handlerFactory.getStatisticsManager();

    return RequestResult(
        new GetPersonalStatisticsResponse(
            sManager.getUserStatistics(
                getUserByInfo(info).getUsername()
            )
        ),

        new MenuRequestHandler(*this)
    );
}

RequestResult MenuRequestHandler::logout(const RequestInfo &info, const ProtocolRequest &) const
{
    LoginManager &uManager = m_handlerFactory.getLoginManager();

    uManager.logout(info.client);

    return RequestResult(new LogoutResponse(), new LoginRequestHandler(this->m_handlerFactory));
}
