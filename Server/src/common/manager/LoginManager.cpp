#include "manager/LoginManager.h"

#include <algorithm>

LoginManager::LoginManager(IDatabase* const database) :
	m_database(database)
{}

SignupResponse LoginManager::signup(const std::string& username, const std::string& password, const std::string& email)
{
	try
	{
		this->m_database->addNewUser(username, password, email);
	}
	catch (std::runtime_error& _) // addNewUser will return runtime_error when adding a user with the same username bc its UNIQUE.
	{
		//TODO: Check what e.what says about that, and act accordingly.
		// Only throw this if relevant, otherwise generic/internal error.
		return SignupResponse(SignupStatus::FAILED_USERNAME_TAKEN);
	}

	const LoginResponse loginRes = this->login(username, password);

	// Simply convert the login response to a signup one
	if (loginRes.status == LoginStatus::SUCCESS)
	{
		return SignupResponse(SignupStatus::SUCCESS, loginRes.userId);
	}

	return SignupResponse(SignupStatus::FAILED_INTERNAL_ERROR);
}


LoginResponse LoginManager::login(const std::string& username, const std::string& password)
{
	const unsigned int userId = this->m_database->getIdOfUser(username, password);

	if (userId == -1)
	{
		return LoginResponse(LoginStatus::FAILED_INVALID_CREDENTIALS);
	}

	if (this->m_loggedUsers.contains(username))
	{
		return LoginResponse(LoginStatus::FAILED_ALREADY_LOGGED_IN);
	}

	this->m_loggedUsers.insert({username, LoggedUser(userId, username)});

	return LoginResponse(LoginStatus::SUCCESS, userId);
}

LogoutResponse LoginManager::logout(const std::string& username)
{
	const auto it = this->m_loggedUsers.find(username);

	if (it == m_loggedUsers.end()) // if found
	{
		return LogoutResponse(LogoutStatus::FAILED_NOT_LOGGED_IN);
	}

	m_loggedUsers.erase(it);
	return LogoutResponse(LogoutStatus::SUCCESS);
}
