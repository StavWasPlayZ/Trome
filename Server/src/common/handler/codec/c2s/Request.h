#pragma once

#include <string>
#include <optional>

#include "infrastructure/request/RequestInfo.h"

enum class RequestCode : unsigned char
{
	LOGIN = 1,
	SIGNUP = 2,
	GET_PLAYERS_IN_ROOM = 3,
	JOIN_ROOM = 4,
	CREATE_ROOM = 5
};

struct ProtocolRequest
{
	/**
	 * Constructs a new ProtocolRequest from the provided request info.
	 *
	 * NOTE: The returned resource must be freed.
	 */
	static ProtocolRequest* fromRequest(RequestInfo& info);
};


struct LoginRequest : ProtocolRequest
{
	LoginRequest(std::string& username, const std::string& password);
	
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
    GetPlayersInRoomRequest(unsigned int roomID);

    const unsigned int roomID;
};

struct JoinRoomRequest : ProtocolRequest
{
    JoinRoomRequest(unsigned int roomID);

    const unsigned int roomID;
};

struct CreateRoomRequest : ProtocolRequest
{
    CreateRoomRequest(
		const std::string roomName,
		unsigned int maxPlayers,
		unsigned int questionCount,
		unsigned int answerTimeout
	);

	const std::string roomName;
    const unsigned int maxPlayers;
    const unsigned int questionCount;
    const unsigned int answerTimeout;
};