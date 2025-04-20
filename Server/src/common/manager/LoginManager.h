#pragma once

#include <unordered_map>

#include "LoggedUser.h"

#include "infrastructure/db/IDatabase.h"
#include "handler/codec/s2c/Response.h"
#include "handler/codec/c2s/Request.h"

class LoginManager
{
public:
	explicit LoginManager(const IDatabase& m_database);

	/**
	 * Signs up a new user, then registers them as logged in.
	 */
	SignupResponse signup(const RequestInfo& context, const SignupRequest& request);

	/**
	 * Registers the provided user as logged in, provided their credentials match.
	 */
	LoginResponse login(const RequestInfo& context, const LoginRequest& request);

	/**
	 * Unregisters the user as being signed in.
	 */
	LogoutResponse logout(const std::string& username);

private:
	const IDatabase& m_database;
	/**
	 * Maps a username to a logged in user.
	 * Mapping as such because the keys are based, for some reason, on usernames only.
	 * 
	 * TODO: Ask if it may be done with IDs.
	 */
	std::unordered_map<std::string, LoggedUser> m_loggedUsers;
};