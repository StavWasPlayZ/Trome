#include "MenuRequestHandler.h"

#include "RequestHandlerFactory.h"
#include "RoomMemberRequestHandler.h"
#include "codec/c2s/Request.h"

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
    const RoomManager &rManager = m_handlerFactory.getRoomManager();

    const std::optional<Room*> room = rManager.getRoom(req.roomID);

    if (!room)
    {
        return RequestResult(
            JsonResponsePacketSerializer::serializeResponse(
                JoinRoomResponse(ConsumingResponseStatus::ERROR_UNKNOWN_RESOURCE)
            ),

            new MenuRequestHandler(*this)
        );
    }

    room.value()->addUser(getUserByInfo(info));
    auto temp = &room.value();

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(
            JoinRoomResponse(ConsumingResponseStatus::SUCCESS)
        ),
        new RoomMemberRequestHandler(this->m_handlerFactory, *room.value())
    );
}

RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo &, const ProtocolRequest &request) const
{
    const GetPlayersInRoomRequest &req = static_cast<const GetPlayersInRoomRequest &>(request);
    const RoomManager &rManager = m_handlerFactory.getRoomManager();

    const std::optional<Room*> room = rManager.getRoom(req.roomID);

    if (!room)
    {
        return RequestResult(
            JsonResponsePacketSerializer::serializeResponse(
                GetPlayersInRoomResponse(ConsumingResponseStatus::ERROR_UNKNOWN_RESOURCE, std::nullopt)
            ),

            new MenuRequestHandler(*this)
        );
    }

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(
            GetPlayersInRoomResponse(ConsumingResponseStatus::SUCCESS, room.value()->getAllUsers())
        ),

        new MenuRequestHandler(*this)
    );
}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& info, const ProtocolRequest& request) const
{
    RoomManager &rManager = m_handlerFactory.getRoomManager();

    const CreateRoomRequest &req = static_cast<const CreateRoomRequest &>(request);

    const RoomData roomData = RoomData(
        req.roomName,
        RoomStatus::WAITING,
        req.maxPlayers,
        req.answerTimeout,
        req.questionCount
    );

    rManager.createRoom(getUserByInfo(info), roomData);

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(
            CreateRoomResponse(GenericResponseStatus::SUCCESS, roomData.id)
        ),

        // TODO : make it RoomAdminRequestHandler when its implemented
        new MenuRequestHandler(*this)
    );
}

RequestResult MenuRequestHandler::getRooms(const RequestInfo &, const ProtocolRequest &) const
{
    const RoomManager &rManager = m_handlerFactory.getRoomManager();

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(
            GetRoomsResponse(GenericResponseStatus::SUCCESS, rManager.getRooms())
        ),

        new MenuRequestHandler(*this)
    );
}

RequestResult MenuRequestHandler::getHighScores(const RequestInfo &, const ProtocolRequest &) const
{
    const StatisticsManager &sManager = m_handlerFactory.getStatisticsManager();

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(
            GetHighScoresResponse(GeneralStatsStatus::SUCCESS, sManager.getHighScores())
        ),

        new MenuRequestHandler(*this)
    );
}

RequestResult MenuRequestHandler::getPersonalStatistics(const RequestInfo& info, const ProtocolRequest &) const
{
    const StatisticsManager &sManager = m_handlerFactory.getStatisticsManager();

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(
            GetPersonalStatisticsResponse(GeneralStatsStatus::SUCCESS,
                sManager.getUserStatistics(
                    getUserByInfo(info).getUsername()
                )
            )
        ),

        new MenuRequestHandler(*this)
    );
}

RequestResult MenuRequestHandler::logout(const RequestInfo &info, const ProtocolRequest &) const
{
    LoginManager &uManager = m_handlerFactory.getLoginManager();

    uManager.logout(info, getUserByInfo(info).getUsername());

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(
            LogoutResponse(LogoutStatus::SUCCESS)
        ),

        new LoginRequestHandler(this->m_handlerFactory)
    );
}
