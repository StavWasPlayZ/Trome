#pragma once

#include "IDatabase.h"

const std::string TABLE_USERS = "users";


// callbacks
int callbackDoesExist(void* data, int argc, char** argv, char** azColName);

class SqliteDatabase : public IDatabase
{
public:
	virtual bool open() override;
	virtual bool close() override;
	virtual bool doesUserExists(const std::string username) const override;
	virtual bool doesPasswordMatch(const std::string username, const std::string password) const override;
	virtual bool addNewUser(const std::string username, const std::string password, const std::string mail) const override;

private:
	sqlite3* _db;
};
