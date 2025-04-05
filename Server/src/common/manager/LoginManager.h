#pragma once

#include "infrastructure/db/IDatabase.h"
#include "Constants.h"
#include <vector>

class LoginManager
{
public:
	/**
	 * Signs up a new user, then registers them as logged in.
	 */
	unsigned char signup(const std::string& username, const std::string& password, const std::string& mail);

	/**
	 * Registers the provided user as logged in, provided their credentials match.
	 */
	unsigned char login(const std::string& username, const std::string& password);

	/**
	 * Unregisters the user as being signed in.
	 */
	void logout(const std::string& username);

private:
	IDatabase* const m_database;
	std::vector<std::string> m_loggedUsers;
};