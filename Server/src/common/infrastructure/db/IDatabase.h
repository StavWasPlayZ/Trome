#pragma once
#include <iostream>

class IDatabase
{
public:
	IDatabase(const std::string dbName);
	virtual ~IDatabase();

	virtual bool open() const = 0;
	virtual bool close() const = 0;
	virtual bool doesUserExists(const std::string username) const = 0;
	virtual bool doesPasswordMatch(const std::string username, const std::string password) const = 0;
	virtual bool addNewUser(const std::string username, const std::string password, const std::string mail) const = 0;

private:
	std::string _dbName;
};
