#pragma once

#include "infrastructure/RoomData.h"

#include <optional>
#include <string>

#include "infrastructure/request/RequestInfo.h"

enum class RequestCode : unsigned char
{
    LOGOUT = 0,
	LOGIN = 1,
	SIGNUP,
	GET_PLAYERS_IN_ROOM,
	JOIN_ROOM,
	CREATE_ROOM,
    GET_ROOMS,
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

struct ProtocolRequest
{
	virtual ~ProtocolRequest();
	
	/**
	 * Constructs a new ProtocolRequest from the provided request info.
	 *
	 * NOTE: The returned resource must be freed.
	 */
    static ProtocolRequest *fromRequest(const RequestInfo &info);
};


struct LoginRequest : ProtocolRequest
{
	LoginRequest(const std::string& username, const std::string& password);
	
	const std::string username;
	const std::string password;
};

struct SignupRequest : LoginRequest
{
	SignupRequest(
		const std::string& username,
		const std::string& password,
		const std::string& email,
		const std::string& phone,
		const std::string& birthdate,
		const std::optional<std::string>& address
	);

	const std::string email;
	const std::string phone;
	const std::optional<std::string> address;
	const std::string birthdate;
};

struct LogoutRequest : ProtocolRequest
{
};

struct GetPlayersInRoomRequest : ProtocolRequest
{
    //NOTE: Removed Room ID parameter from the request, as it is unnecessary.
    // (At least in our version.)
    // explicit GetPlayersInRoomRequest(unsigned int roomID);
    //
    // const unsigned int roomID;
};

struct JoinRoomRequest : ProtocolRequest
{
    explicit JoinRoomRequest(unsigned int roomID);

    const unsigned int roomID;
};

struct GetRoomsRequest : ProtocolRequest
{
};

struct CreateRoomRequest : ProtocolRequest
{
};

struct GetHighScoresRequest : ProtocolRequest
{
};

struct GetPersonalStatisticsRequest : ProtocolRequest
{
};

struct CloseRoomRequest : ProtocolRequest
{
};

struct StartGameRequest : ProtocolRequest
{
    explicit StartGameRequest(const RoomData &data);

    const RoomData data;
};

struct [[deprecated(
    "The Noftifications system has been set in place to allow for automatic, non-polling updates of any "
    "room state changes."
    " This request/response is therefore useless and should not be used."
)]] GetRoomStateRequest : ProtocolRequest
{
};

struct LeaveRoomRequest : ProtocolRequest
{
};

struct UpdateRoomDataRequest : ProtocolRequest
{
    explicit UpdateRoomDataRequest(const RoomData &data);

    const RoomData data;
};

struct LeaveGameRequest : ProtocolRequest
{
};

struct GetQuestionRequest : ProtocolRequest
{
};

struct SubmitAnswerRequest : ProtocolRequest
{
    explicit SubmitAnswerRequest(int answer);

    int answer;
};

struct [[deprecated(
    "The Noftifications system has been set in place to allow for automatic, non-polling updates of any "
    "room state changes."
    " This method is therefore useless and should not be used."
)]] GetGameResultRequest : ProtocolRequest
{
};

struct AddQuestionRequest : ProtocolRequest
{
    AddQuestionRequest(const std::string& question, const std::vector<std::string>& answers);

    const std::string question;
    const std::vector<std::string> answers;
};
