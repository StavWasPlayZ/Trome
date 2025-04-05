#pragma once

#include "IDatabase.h"

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
