#pragma once

#include <string>

struct LoginResponse
{
	LoginResponse(const unsigned int status);
	
	const unsigned int status;
};

struct SignupResponse
{
	SignupResponse(const unsigned int status);
	
	const unsigned int status;
};

struct ErrorResponse
{
	ErrorResponse(const std::string& message);
	
	const std::string message;
};