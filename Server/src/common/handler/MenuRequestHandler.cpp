#include "MenuRequestHandler.h"

#include "RequestHandlerFactory.h"
#include "RoomAdminRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "codec/c2s/request/Request.h"
#include "codec/s2c/response/ErrorResponse.h"
#include "infrastructure/Server.h"

MenuRequestHandler::MenuRequestHandler(const RequestHandlerFactory &handlerFactory) : IRequestHandler(handlerFactory)
{}

std::optional<ErrorStatus> MenuRequestHandler::isRequestRelevant(const RequestInfo &info) const
{
    switch (info.id)
    {
	case RequestCode::JOIN_ROOM:
    case RequestCode::GET_PLAYERS_IN_ROOM:
    case RequestCode::CREATE_ROOM:
    case RequestCode::GET_ROOMS:
    case RequestCode::GET_HIGH_SCORES:
    case RequestCode::GET_USER_STATISTICS:
    case RequestCode::LOGOUT:
    case RequestCode::ADD_QUESTION:
        return std::nullopt;

    default: return ErrorStatus::ILLEGAL_REQUEST;
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
    case RequestCode::GET_USER_STATISTICS:
        return getUserStatistics(info, static_cast<const GetUserStatisticsRequest &>(request));
    case RequestCode::LOGOUT:
        return logout(info, static_cast<const LogoutRequest &>(request));

    // case RequestCode::GET_PLAYERS_IN_ROOM:
    //     return getPlayersInRoom(info, static_cast<const GetPlayersInRoomRequest &>(request));

    case RequestCode::ADD_QUESTION:
        return addQuestion(info, static_cast<const AddQuestionRequest &>(request));

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
            new ErrorResponse(ErrorStatus::UNKNOWN_RESOURCE, info.id)
        );
    }

    const std::vector<LoggedUser*> usersBeforeNew = room.value()->getAllUsers();

    if (usersBeforeNew.size() >= room.value()->getData().maxPlayers)
    {
        return RequestResult(
            new ErrorResponse(ErrorStatus::ROOM_FULL, info.id)
        );
    }

    if (room.value()->getStatus() == RoomStatus::PLAYING)
    {
        return RequestResult(
            new ErrorResponse(ErrorStatus::ROOM_ALREADY_PLAYING, info.id)
        );
    }

    LoggedUser& user = getUserByInfo(info);
    room.value()->addUser(user);

    return RequestResult(
        new JoinRoomResponse(*room.value(), usersBeforeNew),
        new RoomMemberRequestHandler(this->m_handlerFactory, *room.value())
    );
}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& info, const CreateRoomRequest &request) const
{
    RoomManager &rManager = m_handlerFactory.getRoomManager();

    Room& room = rManager.createRoom(
        getUserByInfo(info),
        request.roomType,
        RoomData::ofDefaults(request.roomType)
    );

    return RequestResult(
        new CreateRoomResponse(room.getId(), room.getData()),
        new RoomAdminRequestHandler(m_handlerFactory, room)
    );
}

RequestResult MenuRequestHandler::getRooms(const RequestInfo &, const GetRoomsRequest &) const
{
    const RoomManager &rManager = m_handlerFactory.getRoomManager();
    const std::vector<Room *> rooms = rManager.getWaitingRooms();

    return RequestResult(
        new GetRoomsResponse(rooms)
    );
}

RequestResult MenuRequestHandler::getHighScores(const RequestInfo &, const GetHighScoresRequest &) const
{
    const StatisticsManager &sManager = m_handlerFactory.getStatisticsManager();

    return RequestResult(
        new GetHighScoresResponse(sManager.getHighScores())
    );
}

RequestResult MenuRequestHandler::logout(const RequestInfo &info, const LogoutRequest &) const
{
    LoginManager &uManager = m_handlerFactory.getLoginManager();

    uManager.logout(info.client);

    return RequestResult(new LogoutResponse(), new LoginRequestHandler(this->m_handlerFactory));
}

RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo &info, const GetPlayersInRoomRequest &) const
{
    //NOTICE: This request was entirely replaced by the notification system.
    // It was re-purposed to the Room handler to sync players after a match.
    return RequestResult(new ErrorResponse(ErrorStatus::SERVER_UNIMPLEMENTED, info.id));

    // RoomManager &rManager = m_handlerFactory.getRoomManager();
    //
    // const std::optional<Room*> room = rManager.getRoom(request.roomID);
    //
    // if (!room)
    // {
    //     return RequestResult(
    //         new ErrorResponse(ErrorStatus::UNKNOWN_RESOURCE, info.id)
    //     );
    // }
    //
    // return RequestResult(
    //     new GetPlayersInRoomResponse(room.value()->getAllUsers())
    // );
}

RequestResult MenuRequestHandler::addQuestion(const RequestInfo &info, const AddQuestionRequest &request) const
{
    try
    {
        this->m_handlerFactory.getGameManager().addQuestion(
            request.question,
            getUserByInfo(info)
        );
    }
    catch (const std::runtime_error &e)
    {
        // addQuestions will return runtime_error when adding a user with the same question because it's UNIQUE.
        // Note that the full message reads as follows:
        // "Error in SQL: UNIQUE constraint failed: questions.question"

        if (std::strstr(e.what(), "UNIQUE") != nullptr)
        {
            return RequestResult(new ErrorResponse(ErrorStatus::QUESTION_ALREADY_EXISTS, info.id));
        }

        return RequestResult(new ErrorResponse(ErrorStatus::INVALID_ARGUMENT, info.id, e.what()));
    }

    return RequestResult(new AddQuestionResponse());
}
