#pragma once

#include <unordered_map>

#include "Constants.h"

#include "LoggedUser.h"

#include "infrastructure/db/IDatabase.h"
#include "handler/codec/s2c/Response.h"

class LoginManager
{
public:
	LoginManager(IDatabase* const m_database);

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
	/**
	 * Maps a username to a logged in user.
	 * Mapping as such because the keys are based, for some reason, on usernames only.
	 * 
	 * TODO: Ask if it may be done with IDs.
	 */
	std::unordered_map<std::string, LoggedUser> m_loggedUsers;
};