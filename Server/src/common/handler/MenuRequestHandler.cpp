#include "MenuRequestHandler.h"

#include "RequestHandlerFactory.h"
#include "codec/c2s/Request.h"

MenuRequestHandler::MenuRequestHandler(const RequestHandlerFactory &handlerFactory) : IRequestHandler(handlerFactory)
{}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo &info) const
{
    switch (info.id)
    {
    case RequestCode::CREATE_ROOM:
    case RequestCode::GET_ROOMS:
    case RequestCode::GET_HIGH_SCORES:
    case RequestCode::GET_PERSONAL_STATISTICS:
    case RequestCode::LOGOUT:
        return true;

    default: return false;
    }
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& info, const ProtocolRequest& request) const
{
    switch (info.id)
    {
    case RequestCode::CREATE_ROOM: return createRoom(info, request);
    case RequestCode::GET_ROOMS: return getRooms(info, request);
    case RequestCode::GET_HIGH_SCORES: return getHighScores(info, request);
    case RequestCode::GET_PERSONAL_STATISTICS: return getPersonalStatistics(info, request);
    case RequestCode::LOGOUT: return logout(info, request);

    default: throw std::invalid_argument("Unknown request ID");
    }
}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& info, const ProtocolRequest& request) const
{
    RoomManager &rManager = m_handlerFactory.getRoomManager();
    const LoginManager &uManager = m_handlerFactory.getLoginManager();

    const CreateRoomRequest &req = static_cast<const CreateRoomRequest &>(request);

    const RoomData roomData = RoomData(
        req.roomName,
        RoomStatus::WAITING,
        req.maxPlayers,
        req.answerTimeout,
        req.questionCount
    );

    rManager.createRoom(
        uManager.getUserByClient(info.client),
        roomData
    );

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(
            CreateRoomResponse(GenericRoomResponseStatus::SUCCESS, roomData.id)
        ),

        new MenuRequestHandler(*this)
    );
}

RequestResult MenuRequestHandler::getRooms(const RequestInfo &, const ProtocolRequest &) const
{
    const RoomManager &rManager = m_handlerFactory.getRoomManager();

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(
            GetRoomsResponse(GenericRoomResponseStatus::SUCCESS, rManager.getRooms())
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
    const LoginManager &uManager = m_handlerFactory.getLoginManager();

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(
            GetPersonalStatisticsResponse(GeneralStatsStatus::SUCCESS,
                sManager.getUserStatistics(
                    uManager.getUserByClient(info.client).getUsername()
                )
            )
        ),

        new MenuRequestHandler(*this)
    );
}

RequestResult MenuRequestHandler::logout(const RequestInfo& info, const ProtocolRequest &) const
{
    LoginManager &uManager = m_handlerFactory.getLoginManager();

    uManager.logout(info, uManager.getUserByClient(info.client).getUsername());

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(
            LogoutResponse(LogoutStatus::SUCCESS)
        ),

        new LoginRequestHandler(this->m_handlerFactory)
    );
}
