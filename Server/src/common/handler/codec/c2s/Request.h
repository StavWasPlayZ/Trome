#pragma once

#include <string>

#include <unordered_map>


struct ProtocolRequest {};


struct LoginRequest : ProtocolRequest
{
	LoginRequest(const std::string& username, const std::string& password);
	
	const std::string username;
	const std::string password;
};

struct SignupRequest : ProtocolRequest
{
	SignupRequest(const std::string& username, const std::string& password, const std::string& email);
	
	const std::string username;
	const std::string password;
	const std::string email;
};
