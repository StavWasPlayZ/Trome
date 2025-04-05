#pragma once

#include "infrastructure/db/IDatabase.h"
#include "Constants.h"
#include <vector>

class LoginManager
{
private:
	IDatabase* m_database;
	std::vector<std::string> m_loggedUsers;

public:
	/*
		signs up a user
		returns status to send
	*/
	unsigned char signup(const std::string& username, const std::string& password, const std::string& mail);


	/*
		login a user
		returns status to send
	*/
	unsigned char login(const std::string& username, const std::string& password);


	/*
		logouts a user
	*/
	void logout(const std::string& username);
};