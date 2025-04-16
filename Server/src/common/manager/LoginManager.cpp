#include "manager/LoginManager.h"

#include "exception/RegexViolationException.h"

LoginManager::LoginManager(const IDatabase& database) :
	m_database(database)
{}

SignupResponse LoginManager::signup(const RequestInfo &context, const SignupRequest &request)
{
	try
	{
		this->m_database.addNewUser(
			request.username,
			request.password,
			request.email,
			request.phone,
			request.birthdate,
			request.address
		);
	}
    catch (const RegexViolationException &e)
    {
        //TODO: Add detail as to what regex failed (contained in e.field)
        // Generally add a field for optional failure details.
        return SignupResponse(SignupStatus::FAILED_INVALID_ARGUMENT);
    }
	catch (const std::runtime_error& e)
	{
	    // addNewUser will return runtime_error when adding a user with the same username bc its UNIQUE.

		//TODO: actually check what the error is about, and act accordingly.
		// Only throw this if relevant, otherwise generic/internal error.
		return SignupResponse(SignupStatus::FAILED_USERNAME_TAKEN);
	}

	const LoginResponse loginRes = this->login(context, request);

	// Simply convert the login response to a signup one
	if (loginRes.status == LoginStatus::SUCCESS)
	{
		return SignupResponse(SignupStatus::SUCCESS, loginRes.userId);
	}

	return SignupResponse(SignupStatus::FAILED_INTERNAL_ERROR);
}


LoginResponse LoginManager::login(const RequestInfo &context, const LoginRequest &request)
{
	const unsigned int userId = this->m_database.getIdOfUser(request.username, request.password);

	if (userId == -1)
	{
		return LoginResponse(LoginStatus::FAILED_INVALID_CREDENTIALS);
	}

	if (this->m_loggedUsers.contains(request.username))
	{
		return LoginResponse(LoginStatus::FAILED_ALREADY_LOGGED_IN);
	}

	this->m_loggedUsers.emplace(request.username, LoggedUser(userId, request.username, &context.client));

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
