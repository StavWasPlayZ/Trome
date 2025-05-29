#include "MenuRequestHandler.h"

#include "RequestHandlerFactory.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "codec/c2s/request/Request.h"
#include "codec/s2c/response/ErrorResponse.h"

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
    case RequestCode::JOIN_ROOM:
        return joinRoom(info, static_cast<const JoinRoomRequest &>(request));
    case RequestCode::CREATE_ROOM:
        return createRoom(info, static_cast<const CreateRoomRequest &>(request));
    case RequestCode::GET_ROOMS:
        return getRooms(info, static_cast<const GetRoomsRequest &>(request));
    case RequestCode::GET_HIGH_SCORES:
        return getHighScores(info, static_cast<const GetHighScoresRequest &>(request));
    case RequestCode::GET_PERSONAL_STATISTICS:
        return getPersonalStatistics(info, static_cast<const GetPersonalStatisticsRequest &>(request));
    case RequestCode::LOGOUT:
        return logout(info, static_cast<const LogoutRequest &>(request));

    case RequestCode::GET_PLAYERS_IN_ROOM:
        return getPlayersInRoom(info, static_cast<const GetPlayersInRoomRequest &>(request));

    default: throw std::invalid_argument("Unknown request ID");
    }
}

RequestResult MenuRequestHandler::joinRoom(const RequestInfo &info, const JoinRoomRequest &request) const
{
    RoomManager &rManager = m_handlerFactory.getRoomManager();

    const std::optional<Room*> room = rManager.getRoom(request.roomID);

    if (!room)
    {
        return RequestResult(
            new ErrorResponse(ErrorStatus::ERROR_UNKNOWN_RESOURCE, info.id)
        );
    }

    const std::vector<LoggedUser*> usersBeforeNew = room.value()->getAllUsers();

    LoggedUser& user = getUserByInfo(info);
    room.value()->addUser(user);

    return RequestResult(
        new JoinRoomResponse(*room.value(), usersBeforeNew),
        new RoomMemberRequestHandler(this->m_handlerFactory, *room.value())
    );
}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& info, const CreateRoomRequest &) const
{
    RoomManager &rManager = m_handlerFactory.getRoomManager();

    Room& room = rManager.createRoom(getUserByInfo(info), RoomData::ofDefaults());

    return RequestResult(
        new CreateRoomResponse(room.getId(), room.getData()),
        new RoomAdminRequestHandler(m_handlerFactory, room)
    );
}

RequestResult MenuRequestHandler::getRooms(const RequestInfo &, const GetRoomsRequest &) const
{
    RoomManager &rManager = m_handlerFactory.getRoomManager();

    return RequestResult(
        new GetRoomsResponse(rManager.getRooms())
    );
}

RequestResult MenuRequestHandler::getHighScores(const RequestInfo &, const GetHighScoresRequest &) const
{
    const StatisticsManager &sManager = m_handlerFactory.getStatisticsManager();

    return RequestResult(
        new GetHighScoresResponse(sManager.getHighScores())
    );
}

RequestResult MenuRequestHandler::getPersonalStatistics(const RequestInfo& info, const GetPersonalStatisticsRequest &) const
{
    const StatisticsManager &sManager = m_handlerFactory.getStatisticsManager();

    return RequestResult(
        new GetPersonalStatisticsResponse(
            sManager.getUserStatistics(
                getUserByInfo(info).getUsername()
            )
        )
    );
}

RequestResult MenuRequestHandler::logout(const RequestInfo &info, const LogoutRequest &) const
{
    LoginManager &uManager = m_handlerFactory.getLoginManager();

    uManager.logout(info.client);

    return RequestResult(new LogoutResponse(), new LoginRequestHandler(this->m_handlerFactory));
}

RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo &info, const GetPlayersInRoomRequest &request) const
{
    RoomManager &rManager = m_handlerFactory.getRoomManager();

    const std::optional<Room*> room = rManager.getRoom(request.roomID);

    if (!room)
    {
        return RequestResult(
            new ErrorResponse(ErrorStatus::ERROR_UNKNOWN_RESOURCE, info.id)
        );
    }

    return RequestResult(
        new GetPlayersInRoomResponse(room.value()->getAllUsers())
    );
}