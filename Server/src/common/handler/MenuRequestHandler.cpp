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
    RoomManager &rManager = m_handlerFactory.getRoomManager();
    LoginManager &uManager = m_handlerFactory.getLoginManager();
    StatisticsManager &sManager = m_handlerFactory.getStatisticsManager();

    switch (info.id)
    {
    case RequestCode::CREATE_ROOM: {
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

        //TODO: Probably forward to room handler
        return RequestResult(
            JsonResponsePacketSerializer::serializeResponse(
                CreateRoomResponse(GenericRoomResponseStatus::SUCCESS, roomData.id)
            ),

            new MenuRequestHandler(*this)
        );
    }
    case RequestCode::GET_ROOMS: {
        return RequestResult(
            JsonResponsePacketSerializer::serializeResponse(
                GetRoomsResponse(GenericRoomResponseStatus::SUCCESS, rManager.getRooms())
            ),

            new MenuRequestHandler(*this)
        );
    }
    case RequestCode::GET_HIGH_SCORES: {
        sManager.getHighScore();

        // return RequestResult(
        //     JsonResponsePacketSerializer::serializeResponse(
        //         GetHighScoresResponse(GeneralStatsStatus::SUCCESS, sManager.getHighScore())
        //     ),
        //
        //     new MenuRequestHandler(*this)
        // );
    }
    case RequestCode::GET_PERSONAL_STATISTICS:
    case RequestCode::LOGOUT: {
        uManager.logout(info, uManager.getUserByClient(info.client).getUsername());

        return RequestResult(
            JsonResponsePacketSerializer::serializeResponse(
                LogoutResponse(LogoutStatus::SUCCESS)
            ),

            new LoginRequestHandler(this->m_handlerFactory)
        );
    }

    default: throw std::invalid_argument("Unknown request ID");
    }
}
