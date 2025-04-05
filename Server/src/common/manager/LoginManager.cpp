#include "manager/LoginManager.h"

unsigned char LoginManager::signup(const std::string& username, const std::string& password, const std::string& mail)
{
	try
	{
		this->m_database->addNewUser(username, password, mail);
		return this->login(username, password);
	}
	catch (std::runtime_error& e) // addNewUser will return runtime_error when adding a user with the same username bc its UNIQE
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
