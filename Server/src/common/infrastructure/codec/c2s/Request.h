#pragma once

#include <string>

struct LoginRequest
{
	LoginRequest(const std::string& username, const std::string& password);
	
	const std::string username;
	const std::string password;
};

struct SignupRequest
{
	SignupRequest(const std::string& username, const std::string& password, const std::string& email);
	
	const std::string username;
	const std::string password;
	const std::string email;
};
