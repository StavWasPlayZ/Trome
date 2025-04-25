#pragma once

#include <string>
#include <optional>

#include "infrastructure/request/RequestInfo.h"

#include <infrastructure/RoomData.h>

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
    UPDATE_ROOM_DATA
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

struct GetPlayersInRoomRequest : ProtocolRequest
{
    explicit GetPlayersInRoomRequest(unsigned int roomID);

    const unsigned int roomID;
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
    CreateRoomRequest(
		const std::string &roomName,
		int maxPlayers,
		int questionCount,
		int answerTimeout
	);

	const std::string roomName;
    const int maxPlayers;
    const int questionCount;
    const int answerTimeout;
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
};

struct GetRoomStateRequest : ProtocolRequest
{
};

struct LeaveRoomRequest : ProtocolRequest
{
};

struct UpdateRoomDataRequest : ProtocolRequest
{
    UpdateRoomDataRequest(unsigned int userId, const std::string &roomName, const RoomData &status, int maxPlayers,
                          int timePerQuestion, int questionsCount);

	const unsigned int userId;
    const std::string roomName;
    const RoomData status;
    const int maxPlayers;
    const int timePerQuestion;
    const int questionsCount;
};