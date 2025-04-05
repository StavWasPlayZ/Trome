#include "SqliteDatabase.h"

const std::string SqliteDatabase::TABLE_USERS = "users";

const std::string SqliteDatabase::CREATE_USERS_TBL_QUERY = 
	"CREATE TABLE IF NOT EXISTS " + SqliteDatabase::TABLE_USERS + " ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"username TEXT NOT NULL, "
		"password TEXT NOT NULL, "
		"mail TEXT NOT NULL"
	");";


SqliteDatabase::SqliteDatabase() :
	_dbName("trivia-database")
{}

SqliteDatabase::~SqliteDatabase()
{
	close();
}


bool SqliteDatabase::open()
{
	char* errMessage = nullptr;

	int res = sqlite3_open(this->_dbName.c_str(), &(this->_dbInstance));
	if (res != SQLITE_OK)
	{
		this->_dbInstance = nullptr;
		return false;
	}

	// Initialize the database with the tables.
	// Will not execute if the tables already exist.
	execSql(CREATE_USERS_TBL_QUERY);

	return true;
}

bool SqliteDatabase::close()
{
	if (sqlite3_close(this->_dbInstance) == SQLITE_OK)
	{
		this->_dbInstance = nullptr;
		return true;
	}

	return false;
}

bool SqliteDatabase::doesUserExist(const std::string& username) const
{
	return queryExists(
		"SELECT EXISTS("
			"SELECT 1 FROM " + TABLE_USERS +
			" WHERE "
			"username = '" + username + "'"
		") AS q_exists;"
	);
}

bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
	return queryExists(
		"SELECT EXISTS("
			"SELECT 1 FROM " + TABLE_USERS +
			" WHERE "
			"username = '" + username + "'"
			" AND "
			"password = '" + password + "'"
		") AS q_exists;"
	);
}

unsigned int SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& mail) const
{
	return *querySql<unsigned int>(
		"INSERT INTO " + TABLE_USERS + " (username, password, mail)"
		" VALUES "
		"('" + username + "','" + password + "','" + mail + "')"

		" RETURNING ID;",

		[](const std::map<std::string, std::string> columns) -> unsigned int
		{
			return (unsigned int) std::stoul(columns.at("ID"));
		}
	).begin();
}



// Generic wrapper implementations

bool SqliteDatabase::queryExists(const std::string& query) const
{
	return *querySql<bool>(
		query,
		[](const std::map<std::string, std::string> columns) -> bool
		{
			return columns.at("q_exists") == "1";
		}
	).begin();
}

void SqliteDatabase::execSql(const std::string& query) const
{
	char* errMessage;

	int result = sqlite3_exec(
		this->_dbInstance,
		query.c_str(),
		nullptr,
		nullptr,
		&errMessage
	);

	if (result != SQLITE_OK)
	{
		throw std::runtime_error("Error in SQL: " + std::string(errMessage));
	}
}
