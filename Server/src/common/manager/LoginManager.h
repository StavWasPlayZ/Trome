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
	 *
	 * NOTE: THE RETURNED RESOURCE MUST BE FREED
	 */
	ProtocolResponse* signup(const RequestInfo &info, const SignupRequest &request);

	/**
	 * Registers the provided user as logged in, provided their credentials match.
	 *
	 * NOTE: THE RETURNED RESOURCE MUST BE FREED
	 */
	ProtocolResponse* login(const RequestInfo &info, const LoginRequest &request);

	/**
	 * Unregisters the user as being signed in.
	 *
	 * Returns: True if the user has successfully logged out, or false if they
	 * weren't logged in to begin with.
	 */
	bool logout(const Client &client);

	bool isLoggedIn(const Client &client) const;
    LoggedUser& getUserByClient(const Client& client) const;

private:
	const IDatabase& m_database;
	/**
	 * Maps a username to a logged in user.
	 * Mapping as such because the keys are based, for some reason, on usernames only.
	 * 
	 * TODO: Ask if it may be done with IDs.
	 */
	std::unordered_map<std::string, LoggedUser> m_loggedUsers;

    std::unordered_map<const Client*, LoggedUser*> m_clientToLoggedUser;
};