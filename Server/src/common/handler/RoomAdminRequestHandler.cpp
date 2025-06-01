#include "RoomAdminRequestHandler.h"

#include "GameRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "codec/c2s/request/Request.h"
#include "codec/s2c/response/ErrorResponse.h"
#include "infrastructure/Client.h"
#include "manager/RoomManager.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(const RequestHandlerFactory &handlerFactory, Room& room) :
    RoomRequestHandler(handlerFactory, room)
{}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo &info) const
{
    switch (info.id)
    {
	case RequestCode::START_GAME:
    case RequestCode::CLOSE_ROOM:
    case RequestCode::UPDATE_ROOM_DATA:
    case RequestCode::GET_ROOM_STATE:
        return true;

    default:
        return RoomRequestHandler::isRequestRelevant(info);
    }
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo &info, const ProtocolRequest &request) const
{
    switch (info.id)
    {
    case RequestCode::START_GAME:
        return startGame(info, static_cast<const StartGameRequest &>(request));
    case RequestCode::CLOSE_ROOM:
        return closeRoom(info, static_cast<const CloseRoomRequest &>(request));
    case RequestCode::UPDATE_ROOM_DATA:
        return updateRoomData(info, static_cast<const UpdateRoomDataRequest &>(request));

    default:
        return RoomRequestHandler::handleRequest(info, request);
    }
}

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo &info, const StartGameRequest &request) const
{
    Game& game = this->m_room.createNewGame(this->m_handlerFactory.getGameManager());
    game.startGame();

    setRequestHandlers(
        [this, &game](const LoggedUser *) {
            return new GameRequestHandler(this->m_handlerFactory, game);
        },

        this->m_room.getAllUsers(),
        GameStartedNotification(request.data),
        &getUserByInfo(info)
    );

    return RequestResult(
        new StartGameResponse(),
        new GameRequestHandler(m_handlerFactory, game)
    );
}

RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo &, const CloseRoomRequest &) const
{
    m_handlerFactory.getRoomManager().deleteRoom(m_room);

    return RequestResult(
        new CloseRoomResponse(),
        new MenuRequestHandler(m_handlerFactory)
    );
}

RequestResult RoomAdminRequestHandler::updateRoomData(const RequestInfo &, const UpdateRoomDataRequest &request) const
{
    m_room.setData(request.data);

    return RequestResult(
        new UpdateRoomDataResponse()
    );
}
