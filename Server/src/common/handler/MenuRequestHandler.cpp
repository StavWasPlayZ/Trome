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
    case RequestCode::GET_PLAYER_IN_ROOM:
    case RequestCode::GET_HIGH_SCORES:
    case RequestCode::GET_PERSONAL_STATISTICS:
    case RequestCode::LOGOUT:
        return true;

    default: return false;
    }
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& info, const ProtocolRequest& request) const
{
    RoomManager& rManager = m_handlerFactory.getRoomManager();
    LoginManager& uManager = m_handlerFactory.getLoginManager();

    switch (info.id)
    {
    case RequestCode::CREATE_ROOM: {
        const CreateRoomRequest &req = static_cast<const CreateRoomRequest &>(request);

        rManager.createRoom(
            uManager.getUserByClient(info.client),
            RoomData(
                req.roomName,
                RoomStatus::WAITING,
                req.maxPlayers,
                req.answerTimeout
            )
        );

        // return RequestResult(
        //     JsonResponsePacketSerializer::serializeResponse(
        //         CreateRoomResponse(GeneralRoomStatus::SUCCESS, rManager.getRooms())
        //     ),
        //
        //     new MenuRequestHandler(*this)
        // );
    }
    case RequestCode::GET_ROOMS: {
        return RequestResult(
            JsonResponsePacketSerializer::serializeResponse(
                GetRoomsResponse(GeneralRoomStatus::SUCCESS, rManager.getRooms())
            ),

            new MenuRequestHandler(*this)
        );
    }
    case RequestCode::GET_PLAYER_IN_ROOM:
    case RequestCode::GET_HIGH_SCORES:
    case RequestCode::GET_PERSONAL_STATISTICS:
    case RequestCode::LOGOUT:

    default: throw std::invalid_argument("Unknown request ID");
    }
}
