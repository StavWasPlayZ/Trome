#include "manager/LoginManager.h"

#include <algorithm>

unsigned char LoginManager::signup(const std::string& username, const std::string& password, const std::string& mail)
{
	try
	{
		this->m_database->addNewUser(username, password, mail);
		return this->login(username, password);
	}
	catch (std::runtime_error& e) // addNewUser will return runtime_error when adding a user with the same username bc its UNIQUE.
	{
		return (unsigned char)ProtocolCode::ERROR;
	}
}


unsigned char LoginManager::login(const std::string& username, const std::string& password)
{
	if (!(this->m_database->doesPasswordMatch(username, password)))
	{
		return (unsigned char)ProtocolCode::ERROR;
	}

	this->m_loggedUsers.push_back(username);
	return (unsigned char)ProtocolCode::OK;
}

void LoginManager::logout(const std::string& username)
{
	auto it = std::find(m_loggedUsers.begin(), m_loggedUsers.end(), username); // finds username

	if (it != m_loggedUsers.end()) // if found
	{
		m_loggedUsers.erase(it);
	}
}
