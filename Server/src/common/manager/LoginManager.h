#pragma once

#include "infrastructure/db/IDatabase.h"
#include <vector>

class LoginManager
{
private:
	IDatabase* m_database;
	std::vector<std::string> m_loggedUsers;

public:
	unsigned char signup(const std::string& username, const std::string& password, const std::string& mail);
	unsigned char login(const std::string& username, const std::string& password);
	void logout(const std::string& username);
};