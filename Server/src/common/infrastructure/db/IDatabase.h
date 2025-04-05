#pragma once
#include <iostream>
#include "sqlite3.h"
#include <io.h>

class IDatabase
{
public:
	IDatabase(const std::string dbName);
	virtual ~IDatabase();

	virtual bool open() = 0;
	virtual bool close() = 0;
	virtual bool doesUserExists(const std::string username) const = 0;
	virtual bool doesPasswordMatch(const std::string username, const std::string password) const = 0;
	virtual bool addNewUser(const std::string username, const std::string password, const std::string mail) const = 0;

protected:
	std::string _dbName;
};
