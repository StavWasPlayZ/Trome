#pragma once

#include <string>


struct ProtocolResponse {};


struct LoginResponse : ProtocolResponse
{
	LoginResponse(const unsigned int status);
	
	const unsigned int status;
};

struct SignupResponse : ProtocolResponse
{
	SignupResponse(const unsigned int status);
	
	const unsigned int status;
};

struct ErrorResponse : ProtocolResponse
{
	ErrorResponse(const std::string& message);
	
	const std::string message;
};