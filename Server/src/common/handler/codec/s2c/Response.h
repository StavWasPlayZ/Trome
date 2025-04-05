#pragma once

#include <string>


/**
 * S - The enum Status type
 */
template <typename S>
struct ProtocolResponse
{
	ProtocolResponse(const S status);

	const S status;
};


/**
 * S - The enum Status type
 */
template <typename S>
struct RegistrationResponse : ProtocolResponse<S>
{
	RegistrationResponse(const S status, const unsigned int userId);
	/**
	 * A failed login response. Provided no user ID.
	 */
	RegistrationResponse(const S status);

	/**
	 * For failure, equals -1 (4294967295).
	 */
	const unsigned int userId;
};


enum class LoginStatus : unsigned char
{
	SUCCESS = 1,
	FAILED_INVALID_CREDENTIALS = 2,
	FAILED_INTERNAL_ERROR = 0
};

struct LoginResponse : RegistrationResponse<LoginStatus>
{
	LoginResponse(const LoginStatus status, const unsigned int userId);
	/**
	 * A failed login response. Provided no user ID.
	 */
	LoginResponse(const LoginStatus status);
};


enum class SignupStatus : unsigned char
{
	SUCCESS = 1,
	FAILED_USERNAME_TAKEN = 2,
	FAILED_INTERNAL_ERROR = 0
};

struct SignupResponse : RegistrationResponse<SignupStatus>
{
	SignupResponse(const SignupStatus status, const unsigned int userId);
	/**
	 * A failed login response. Provided no user ID.
	 */
	SignupResponse(const SignupStatus status);
};


enum class LogoutStatus : unsigned char
{
	SUCCESS = 1,
	FAILED_NOT_LOGGED_IN = 2,
	FAILED_INTERNAL_ERROR = 0
};

struct LogoutResponse : ProtocolResponse<LogoutStatus>
{
	LogoutResponse(const LogoutStatus status);
};


enum class ErrorStatus : unsigned char
{
	GENERIC = 0,
	SERVER_UNIMPLEMENTED,
	ILLEGAL_REQUEST
};

struct ErrorResponse : ProtocolResponse<ErrorStatus>
{
	ErrorResponse(const ErrorStatus status, const std::string& message);
	
	const std::string message;
};


#include "Response.tpp"
