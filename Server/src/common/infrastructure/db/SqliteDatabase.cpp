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
		std::string str = "CREATE TABLE users ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT, "
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
