#pragma once

#include <string>


struct ProtocolResponse {};

//TODO: Unite LoginResponse & SignupResponse under RegistrationResponse
struct LoginResponse : ProtocolResponse
{
	enum class Status : unsigned char
	{
		SUCCESS = 1,
		FAILED_INVALID_CREDENTIALS = 2,
		FAILED_INTERNAL_ERROR = 0
	};

	LoginResponse(const LoginResponse::Status status, const unsigned int userId);
	/**
	 * A failed login response. Provided no user ID.
	 */
	LoginResponse(const LoginResponse::Status status);

	const Status status;
	/**
	 * For failure, equals -1 (4294967295).
	 */
	const unsigned int userId;
};

struct SignupResponse : ProtocolResponse
{
	enum class Status : unsigned char
	{
		SUCCESS = 1,
		FAILED_USERNAME_TAKEN = 2,
		FAILED_INTERNAL_ERROR = 0
	};

	SignupResponse(const SignupResponse::Status status, const unsigned int userId);
	/**
	 * A failed login response. Provided no user ID.
	 */
	SignupResponse(const SignupResponse::Status status);

	const Status status;
	/**
	 * For failure, equals -1 (4294967295).
	 */
	const unsigned int userId;
};

struct ErrorResponse : ProtocolResponse
{
	ErrorResponse(const std::string& message);
	
	const std::string message;
};