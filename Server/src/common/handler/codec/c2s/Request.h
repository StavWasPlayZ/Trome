#pragma once

#include <string>
#include <optional>

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
    GET_PLAYER_IN_ROOM,
    GET_HIGH_SCORES,
    GET_PERSONAL_STATISTICS
};

struct ProtocolRequest
{
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

struct GetRoomRequest : ProtocolRequest
{
    explicit GetRoomRequest(unsigned int roomID);

    const unsigned int roomID;
};

struct GetRoomsRequest : ProtocolRequest
{
};

struct CreateRoomRequest : ProtocolRequest
{
    CreateRoomRequest(
		const std::string &roomName,
		unsigned int maxPlayers,
		unsigned int questionCount,
		unsigned int answerTimeout
	);

	const std::string roomName;
    const unsigned int maxPlayers;
    const unsigned int questionCount;
    const unsigned int answerTimeout;
};

struct GetHighScoresRequest : ProtocolRequest
{
};

struct GetPersonalStatisticsRequest : ProtocolRequest
{
};
