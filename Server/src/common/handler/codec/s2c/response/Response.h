#pragma once

#include "infrastructure/Game.h"
#include "infrastructure/PlayerResult.h"
#include "infrastructure/UserStatistics.h"

#include <infrastructure/RoomData.h>
#include <optional>
#include <string>
#include <vector>

#ifdef ERROR
#undef ERROR
#endif

class Room;
class LoggedUser;

enum class ResponseCode : unsigned char
{
	ERROR = 0,
	LOGIN,
	SIGNUP,
	LOGOUT,
	JOIN_ROOM,
	CREATE_ROOM,
	GET_ROOMS,
	GET_PLAYERS_IN_ROOM,
	GET_HIGH_SCORES,
	GET_PERSONAL_STATISTICS,
	CLOSE_ROOM,
	START_GAME,
	GET_ROOM_STATE,
	LEAVE_ROOM,
	UPDATE_ROOM_DATA,
    LEAVE_GAME,
    GET_QUESTION,
    SUBMIT_ANSWER,
    GET_GAME_RESULT,
    ADD_QUESTION
};


struct ProtocolResponse
{
    explicit ProtocolResponse(ResponseCode id);
    virtual ~ProtocolResponse();

    const ResponseCode id;
};


struct RegistrationResponse : ProtocolResponse
{
	RegistrationResponse(ResponseCode id, unsigned int userId);

	const unsigned int userId;
};

struct LoginResponse : RegistrationResponse
{
    explicit LoginResponse(unsigned int userId);
};


struct SignupResponse : RegistrationResponse
{
    explicit SignupResponse(unsigned int userId);
};


struct LogoutResponse : ProtocolResponse
{
    LogoutResponse();
};


struct JoinRoomResponse : ProtocolResponse
{
    explicit JoinRoomResponse(const Room& room, const std::vector<LoggedUser*>& players);

	const Room& room;
    const std::vector<LoggedUser*> players;
};

struct CreateRoomResponse : ProtocolResponse
{
    CreateRoomResponse(unsigned int roomId, const RoomData& data);

    const unsigned int roomId;
    RoomData data;
};

struct GetRoomsResponse : ProtocolResponse
{
    explicit GetRoomsResponse(const std::vector<Room*> &rooms);

	const std::vector<Room*> rooms;
};

struct GetPlayersInRoomResponse : ProtocolResponse
{
    explicit GetPlayersInRoomResponse(const std::vector<LoggedUser*> &players);

	const std::vector<LoggedUser*> players;
};

struct GetHighScoresResponse : ProtocolResponse
{
    explicit GetHighScoresResponse(const std::vector<std::pair<std::string, int>> &stats);

    const std::vector<std::pair<std::string, int>> stats;
};

struct GetPersonalStatisticsResponse : ProtocolResponse
{
    explicit GetPersonalStatisticsResponse(const UserStatistics &stats);

    const UserStatistics stats;
};

struct CloseRoomResponse : ProtocolResponse
{
    CloseRoomResponse();
};

struct StartGameResponse : ProtocolResponse
{
    StartGameResponse();
};

struct LeaveRoomResponse : ProtocolResponse
{
    LeaveRoomResponse();
};

struct [[deprecated(
    "The Noftifications system has been set in place to allow for automatic, non-polling updates of any "
    "room state changes."
    " This request/response is therefore useless and should not be used."
)]] GetRoomStateResponse : ProtocolResponse
{
    explicit GetRoomStateResponse(const Room& room);

	const Room& room;
};

struct UpdateRoomDataResponse : ProtocolResponse
{
    UpdateRoomDataResponse();
};

struct LeaveGameResponse : ProtocolResponse
{
    LeaveGameResponse();
};

struct GetQuestionResponse : ProtocolResponse
{
    explicit GetQuestionResponse(const std::optional<UserQuestion> &question, int points);

    const int points;

    /**
     * Empty for if there are no more questions.
     */
    const std::optional<UserQuestion> question;
};

struct SubmitAnswerResponse : ProtocolResponse
{
    SubmitAnswerResponse(const std::optional<UserQuestion> &newQuestion, int points);

    const int points;

    /**
     * Empty for if there are no more questions.
     */
    const std::optional<UserQuestion> newQuestion;
};

struct [[deprecated(
    "The Noftifications system has been set in place to allow for automatic, non-polling updates of any "
    "room state changes."
    " This method is therefore useless and should not be used."
)]] GetGameResultResponse : ProtocolResponse
{
    explicit GetGameResultResponse(const std::vector<PlayerResult>& results);

    const std::vector<PlayerResult> results;
};

struct AddQuestionResponse : ProtocolResponse
{
    AddQuestionResponse();
};
