#pragma once

#include <string>
#include <vector>
#include "infrastructure/Room.h"

enum class ResponseCode : unsigned char
{
	ERROR = 0,
	LOGIN = 1,
	SIGNUP = 2
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

enum class GeneralRoomStatus : unsigned int
{
	SUCCESS = 1,
	ERROR = 0
};

struct JoinRoomResponse : ProtocolResponse<GeneralRoomStatus>
{
    explicit JoinRoomResponse(const unsigned int status);
};

struct CreateRoomResponse : ProtocolResponse<GeneralRoomStatus>
{
    explicit CreateRoomResponse(const unsigned int status);
};

struct GetRoomsResponse : ProtocolResponse<GeneralRoomStatus>
{
    GetRoomsResponse(const unsigned int status, std::vector<RoomData> rooms);

	const std::vector<RoomData> rooms;
};
#include "Response.tpp"
