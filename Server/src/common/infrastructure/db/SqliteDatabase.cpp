#include "SqliteDatabase.h"

/*
	opens the DB
*/
bool SqliteDatabase::open()
{
	char* errMessage = nullptr;

	int file_exists = _access(this->_dbName.c_str(), 0);
	int res = sqlite3_open(this->_dbName.c_str(), &(this->_db));
	if (res != SQLITE_OK)
	{
		this->_db = nullptr;
		return false;
	}

	if (file_exists != 0)
	{
		std::string str = "CREATE TABLE " + TABLE_USERS +
			" (id INTEGER PRIMARY KEY AUTOINCREMENT, "
			"username TEXT NOT NULL, ";
			"password TEXT NOT NULL, ";
			"mail TEXT NOT NULL);";

		res = sqlite3_exec(this->_db, str.c_str(), nullptr, nullptr, &errMessage);

		if (res != SQLITE_OK)
		{
			this->close();
			return false;
		}
	}


	return true;
}

/*
	closes the DB
*/
bool SqliteDatabase::close()
{
	bool done = sqlite3_close(this->_db) == SQLITE_OK;
	this->_db = nullptr;
	return done;
}

/*
	checks if a user exists in db
*/
bool SqliteDatabase::doesUserExists(const std::string username) const
{
	std::string str = "SELECT * FROM " + TABLE_USERS + " WHERE username = '" + username + "';";
	char* errMessage = nullptr;
	bool result = false;
	int res = sqlite3_exec(_db, str.c_str(), callbackDoesExist, &result, &errMessage);

	if (res != SQLITE_OK)
	{
		throw std::runtime_error("Error getting users table");
	}

	return result;
}

/*
	checks if a user have that password
*/
bool SqliteDatabase::doesPasswordMatch(const std::string username, const std::string password) const
{
	std::string str = "SELECT * FROM " + TABLE_USERS + " WHERE username = '" + username + "' AND password = '" + password + "';";
	char* errMessage = nullptr;
	bool result = false;
	int res = sqlite3_exec(_db, str.c_str(), callbackDoesExist, &result, &errMessage);

	if (res != SQLITE_OK)
	{
		throw std::runtime_error("Error getting users table");
	}

	return result;
}

/*
	adds new user
*/
bool SqliteDatabase::addNewUser(const std::string username, const std::string password, const std::string mail) const
{
	std::string str = "INSERT INTO " + TABLE_USERS + " (username, password, mail) VALUES ('" + username + "','" + password + "','" + mail + "';";
	char* errMessage = nullptr;

	int res = sqlite3_exec(_db, str.c_str(), nullptr, nullptr, &errMessage);

	if (res != SQLITE_OK)
	{
		throw std::runtime_error("Error inserting into users table");
	}

	return true;
}

/*
	the callback used to check if some thing is in db
	data - bool*
*/
int callbackDoesExist(void* data, int argc, char** argv, char** azColName)
{
	bool* result = (bool*)data;
	*result = true;
}
