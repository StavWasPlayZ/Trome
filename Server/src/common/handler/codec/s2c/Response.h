#pragma once

#include "infrastructure/UserStatistics.h"

#include <infrastructure/RoomData.h>
#include <optional>
#include <string>
#include <vector>

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
	GET_PLAYER_IN_ROOM,
	GET_HIGH_SCORES,
	GET_PERSONAL_STATISTICS,
	CLOSE_ROOM,
	START_GAME,
	GET_ROOM_STATE,
	LEAVE_ROOM,
	UPDATE_ROOM_DATA
};

// Generic statuses

enum class GenericResponseStatus : unsigned int
{
    SUCCESS = 1,
    ERROR_INTERNAL = 0
};

/**
 * The status of a response that takes in a resource.
 *
 * May be of any type.
 */
enum class ConsumingResponseStatus : unsigned char
{
    SUCCESS = 1,
    ERROR_UNKNOWN_RESOURCE,
    ERROR_INTERNAL = 0,
};


/**
 * S - The enum Status type
 */
template <typename S>
struct ProtocolResponse
{
	explicit ProtocolResponse(S status);

	const S status;
};


/**
 * S - The enum Status type
 */
template <typename S>
struct RegistrationResponse : ProtocolResponse<S>
{
	RegistrationResponse(S status, unsigned int userId);
	/**
	 * A failed login response. Provided no user ID.
	 */
	explicit RegistrationResponse(S status);

	/**
	 * For failure, equals -1 (4294967295).
	 */
	const unsigned int userId;
};


enum class LoginStatus : unsigned char
{
	SUCCESS = 1,
	FAILED_INVALID_CREDENTIALS = 2,
	FAILED_ALREADY_LOGGED_IN = 3,
	FAILED_INTERNAL_ERROR = 0
};

struct LoginResponse : RegistrationResponse<LoginStatus>
{
	LoginResponse(LoginStatus status, unsigned int userId);
	/**
	 * A failed login response. Provided no user ID.
	 */
	explicit LoginResponse(LoginStatus status);
};


enum class SignupStatus : unsigned char
{
	SUCCESS = 1,
	FAILED_USERNAME_TAKEN = 2,
    FAILED_INVALID_ARGUMENT = 3,
	FAILED_INTERNAL_ERROR = 0
};

struct SignupResponse : RegistrationResponse<SignupStatus>
{
	SignupResponse(SignupStatus status, unsigned int userId);
	/**
	 * A failed login response. Provided no user ID.
	 */
	explicit SignupResponse(SignupStatus status);
};


enum class LogoutStatus : unsigned char
{
	SUCCESS = 1,
	FAILED_NOT_LOGGED_IN = 2,
	FAILED_INTERNAL_ERROR = 0
};

struct LogoutResponse : ProtocolResponse<LogoutStatus>
{
	explicit LogoutResponse(LogoutStatus status);
};


enum class ErrorStatus : unsigned char
{
	GENERIC = 0,
	SERVER_UNIMPLEMENTED,
	ILLEGAL_REQUEST
};

struct ErrorResponse : ProtocolResponse<ErrorStatus>
{
	ErrorResponse(ErrorStatus status, const std::string& message);
	
	const std::string message;
};

//TODO: Provide room metadata
struct JoinRoomResponse : ProtocolResponse<ConsumingResponseStatus>
{
    explicit JoinRoomResponse(ConsumingResponseStatus status);
};

struct CreateRoomResponse : ProtocolResponse<GenericResponseStatus>
{
    explicit CreateRoomResponse(GenericResponseStatus status, unsigned int roomId);

    const unsigned int roomId;
};

struct GetRoomsResponse : ProtocolResponse<GenericResponseStatus>
{
    GetRoomsResponse(GenericResponseStatus status, const std::vector<Room*> &rooms);

	const std::vector<Room*> rooms;
};

struct GetPlayersInRoomResponse : ProtocolResponse<ConsumingResponseStatus>
{
    GetPlayersInRoomResponse(ConsumingResponseStatus status, const std::optional<std::vector<LoggedUser*>> &players);

	const std::optional<std::vector<LoggedUser*>> players;
};

enum class GeneralStatsStatus : unsigned int
{
    SUCCESS = 1,
    ERROR = 0,
	NOT_CONNECTED_ERROR = 2
};

struct GetHighScoresResponse : ProtocolResponse<GeneralStatsStatus>
{
    GetHighScoresResponse(GeneralStatsStatus status, const std::vector<std::pair<std::string, int>> &stats);

    const std::vector<std::pair<std::string, int>> stats;
};

struct GetPersonalStatisticsResponse : ProtocolResponse<GeneralStatsStatus>
{
    GetPersonalStatisticsResponse(GeneralStatsStatus status, const UserStatistics &stats);

    const UserStatistics stats;
};

struct CloseRoomResponse : ProtocolResponse<GenericResponseStatus>
{
    explicit CloseRoomResponse(GenericResponseStatus status);
};

struct StartGameResponse : ProtocolResponse<GenericResponseStatus>
{
    explicit StartGameResponse(GenericResponseStatus status);
};

struct LeaveRoomResponse : ProtocolResponse<GenericResponseStatus>
{
    explicit LeaveRoomResponse(GenericResponseStatus status);
};

struct GetRoomStateResponse : ProtocolResponse<GenericResponseStatus>
{
    GetRoomStateResponse(GenericResponseStatus protocolStatus, RoomStatus roomStatus, bool hasGameBegan,
                         const std::vector<LoggedUser*>& players, int answerCount, int answerTimeOut);

	RoomStatus roomStatus;
    bool hasGameBegan;
    std::vector<LoggedUser*> players;
    int answerCount;
    int answerTimeOut;
};

struct UpdateRoomDataResponse : ProtocolResponse<GenericResponseStatus>
{
    explicit UpdateRoomDataResponse(GenericResponseStatus status);
};


#include "Response.tpp"
