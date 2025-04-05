#pragma once

#include <vector>

#include "Constants.h"

#include "infrastructure/db/IDatabase.h"
#include "handler/codec/s2c/Response.h"

class LoginManager
{
public:
	/**
	 * Signs up a new user, then registers them as logged in.
	 */
	SignupResponse signup(const std::string& username, const std::string& password, const std::string& email);

	/**
	 * Registers the provided user as logged in, provided their credentials match.
	 */
	LoginResponse login(const std::string& username, const std::string& password);

	/**
	 * Unregisters the user as being signed in.
	 */
	LogoutResponse logout(const std::string& username);

private:
	IDatabase* const m_database;
	std::vector<std::string> m_loggedUsers;
};