#include "manager/LoginManager.h"

#include "exception/RegexViolationException.h"

LoginManager::LoginManager(const IDatabase& database) :
	m_database(database)
{}

ProtocolResponse *LoginManager::signup(const RequestInfo &context, const SignupRequest &request)
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
        return new ErrorResponse(ErrorStatus::FAILED_INVALID_ARGUMENT, context.id, e.field);
    }
	catch (const std::runtime_error& e)
	{
	    // addNewUser will return runtime_error when adding a user with the same username because it's UNIQUE.

		//TODO: actually check what the error is about, and act accordingly.
		// Only throw this if relevant, otherwise generic/internal error.
		return new ErrorResponse(ErrorStatus::FAILED_USERNAME_TAKEN, context.id);
	}

	const ProtocolResponse* loginRes = this->login(context.client, request);

	// Simply convert the login response to a signup one
	if (loginRes->id != ResponseCode::ERROR)
	{
		SignupResponse* result = new SignupResponse(static_cast<const LoginResponse*>(loginRes)->userId);
	    delete loginRes;
	    return result;
	}

    delete loginRes;

	return new ErrorResponse(ErrorStatus::INTERNAL, context.id);
}


LoginResponse LoginManager::login(const Client &client, const LoginRequest &request)
{
	const unsigned int userId = this->m_database.queryIdOfUser(request.username, request.password);

	if (userId == -1)
	{
		return LoginResponse(LoginStatus::FAILED_INVALID_CREDENTIALS);
	}

	if (this->m_loggedUsers.contains(request.username))
	{
		return LoginResponse(LoginStatus::FAILED_ALREADY_LOGGED_IN);
	}

	const auto result = this->m_loggedUsers.emplace(
	    request.username,
        LoggedUser(userId, request.username, &client)
	);

    this->m_clientToLoggedUser.emplace(&client, &result.first->second);

	return LoginResponse(LoginStatus::SUCCESS, userId);
}

LogoutResponse LoginManager::logout(const Client &client)
{
    const auto it = this->m_clientToLoggedUser.find(&client);

    if (it == m_clientToLoggedUser.end()) // if found
    {
        return LogoutResponse(LogoutStatus::FAILED_NOT_LOGGED_IN);
    }

    this->m_loggedUsers.erase(it->second->getUsername());
    this->m_clientToLoggedUser.erase(&client);

    return LogoutResponse(LogoutStatus::SUCCESS);
}

LoggedUser &LoginManager::getUserByClient(const Client &client) const
{
    return *this->m_clientToLoggedUser.at(&client);
}
