#include "SqliteDatabase.h"

bool SqliteDatabase::open()
{
	char* errMessage = nullptr;

	int file_exists = _access(_dbName.c_str(), 0);
	int res = sqlite3_open(_dbName.c_str(), &_db);
	if (res != SQLITE_OK)
	{
		_db = nullptr;
		return false;
	}

	if (file_exists != 0)
	{
		std::string str = "CREATE TABLE users ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT, "
			"username TEXT NOT NULL, ";
			"password TEXT NOT NULL, ";
			"mail TEXT NOT NULL);";

		res = sqlite3_exec(_db, str.c_str(), nullptr, nullptr, &errMessage);

		if (res != SQLITE_OK)
		{
			this->close();
			return false;
		}
	}


	return true;
}
