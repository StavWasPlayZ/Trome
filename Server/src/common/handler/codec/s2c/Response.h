#pragma once

#include <string>


struct ProtocolResponse {};

/**
 * S - The enum Status type
 */
template <typename S>
struct RegistrationResponse : ProtocolResponse
{
	RegistrationResponse(const S status, const unsigned int userId);
	/**
	 * A failed login response. Provided no user ID.
	 */
	RegistrationResponse(const S status);

	const S status;
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


struct ErrorResponse : ProtocolResponse
{
	ErrorResponse(const std::string& message);
	
	const std::string message;
};


#include "Response.tpp"
