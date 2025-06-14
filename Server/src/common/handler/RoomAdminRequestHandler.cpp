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

std::optional<ErrorStatus> RoomAdminRequestHandler::isRequestRelevant(const RequestInfo &info) const
{
    switch (info.id)
    {
	case RequestCode::START_GAME:
    case RequestCode::CLOSE_ROOM:
    case RequestCode::UPDATE_ROOM_DATA:
    case RequestCode::GET_ROOM_STATE:
    case RequestCode::KICK_PLAYER:
        return std::nullopt;

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
    case RequestCode::KICK_PLAYER:
        return kick(info, static_cast<const KickPlayerRequest &>(request));

    default:
        return RoomRequestHandler::handleRequest(info, request);
    }
}

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo &info, const StartGameRequest &request) const
{
    Game& game = this->m_room.createNewGame(this->m_handlerFactory.getGameManager());
    game.startGame();

    const GameStartedNotification notification = GameStartedNotification(request.data);

    setRequestHandlers(
        [this, &game](const LoggedUser *const) {
            return new GameRequestHandler(this->m_handlerFactory, game);
        },
        this->m_room.getAllUsers(),
        &notification,
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

RequestResult RoomAdminRequestHandler::updateRoomData(const RequestInfo &info, const UpdateRoomDataRequest &request) const
{
    const RoomData& data = request.data;

    //NOTE: nzp = non-zero positive
    if (data.timePerQuestionSecs <= 0)
    {
        return RequestResult(
            new ErrorResponse(ErrorStatus::INVALID_ARGUMENT, info.id, "time_per_question_secs-npz")
        );
    }

    if (data.maxPlayers <= 0)
    {
        return RequestResult(
            new ErrorResponse(ErrorStatus::INVALID_ARGUMENT, info.id, "max_players-nzp")
        );
    }

    if (data.questionsCount <= 0)
    {
        return RequestResult(
            new ErrorResponse(ErrorStatus::INVALID_ARGUMENT, info.id, "questions_count-npz")
        );
    }

    m_room.setData(data);

    return RequestResult(
        new UpdateRoomDataResponse()
    );
}

RequestResult RoomAdminRequestHandler::kick(const RequestInfo &info, const KickPlayerRequest &request) const
{
    if (getUserByInfo(info).getId() == request.userId)
    {
        return RequestResult(new ErrorResponse(ErrorStatus::UNKICKABLE_ENTITY, info.id));
    }

    LoggedUser &toBeKicked = m_handlerFactory.getLoginManager().getUserById(request.userId);
    m_room.kickUser(toBeKicked);

    return RequestResult(new KickPlayerResponse());
};
