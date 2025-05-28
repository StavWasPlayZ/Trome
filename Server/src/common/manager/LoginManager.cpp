#include "manager/LoginManager.h"

// for using our ERROR Response Code
#ifdef ERROR
#undef ERROR
#endif

#include "exception/RegexViolationException.h"
#include "handler/codec/s2c/response/ErrorResponse.h"

LoginManager::LoginManager(const IDatabase& database) :
	m_database(database)
{}

ProtocolResponse *LoginManager::signup(const RequestInfo &info, const SignupRequest &request)
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
        return new ErrorResponse(ErrorStatus::FAILED_INVALID_ARGUMENT, info.id, e.field);
    }
	catch (const std::runtime_error& e)
	{
	    // addNewUser will return runtime_error when adding a user with the same username because it's UNIQUE.
	    // Note that the full message reads as follows:
	    // "Error in SQL: UNIQUE constraint failed: users.username"

        if (std::strstr(e.what(), "UNIQUE") != nullptr)
        {
            return new ErrorResponse(ErrorStatus::FAILED_USERNAME_TAKEN, info.id);
        }

        return new ErrorResponse(ErrorStatus::FAILED_INVALID_ARGUMENT, info.id, e.what());
	}

	const ProtocolResponse *const loginRes = this->login(info, request);

	// Simply convert the login response to a signup one
	if (loginRes->id != ResponseCode::ERROR)
	{
		SignupResponse* result = new SignupResponse(static_cast<const LoginResponse*>(loginRes)->userId);
	    delete loginRes;
	    return result;
	}

    delete loginRes;

	return new ErrorResponse(ErrorStatus::INTERNAL, info.id);
}

ProtocolResponse *LoginManager::login(const RequestInfo &info, const LoginRequest &request)
{
	const unsigned int userId = this->m_database.queryIdOfUser(request.username, request.password);

	if (userId == -1)
	{
        return new ErrorResponse(ErrorStatus::FAILED_INVALID_CREDENTIALS, info.id);
	}

    // If either the client is logged in or the requested user is logged in
	if (isLoggedIn(info.client) || isLoggedIn(userId))
	{
        return new ErrorResponse(ErrorStatus::FAILED_ALREADY_LOGGED_IN, info.id);
	}

	const auto result = this->m_loggedUsers.emplace(
	    request.username,
        LoggedUser(userId, request.username, info.client)
	);

    this->m_clientToLoggedUser.emplace(&info.client, &result.first->second);
    this->m_loggedUsersById.emplace(userId, &result.first->second);

	return new LoginResponse(userId);
}

bool LoginManager::logout(const Client &client)
{
    const auto it = this->m_clientToLoggedUser.find(&client);

    if (it == m_clientToLoggedUser.end())
        return false;

    const unsigned int userId = it->second->getId();

    // This one actually contains the user resource
    this->m_loggedUsers.erase(it->second->getUsername());

    this->m_clientToLoggedUser.erase(&client);
    this->m_loggedUsersById.erase(userId);

    return true;
}

bool LoginManager::isLoggedIn(const Client &client) const
{
    return this->m_clientToLoggedUser.contains(&client);
}

LoggedUser &LoginManager::getUserByClient(const Client &client) const
{
    return *this->m_clientToLoggedUser.at(&client);
}

bool LoginManager::isLoggedIn(const unsigned int id) const
{
    return this->m_loggedUsersById.contains(id);
}
