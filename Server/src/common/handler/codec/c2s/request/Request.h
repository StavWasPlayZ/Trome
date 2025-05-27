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

struct LogoutRequest : ProtocolRequest
{
};

struct [[deprecated(
    "The Noftifications system has been set in place to allow for automatic, non-polling updates of any "
    "room state changes."
    " This request/response is therefore useless and should not be used."
)]] GetPlayersInRoomRequest : ProtocolRequest
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
