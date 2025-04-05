#include "SqliteDatabase.h"

const std::string SqliteDatabase::TABLE_USERS = "users";

const std::string SqliteDatabase::CREATE_USERS_TBL_QUERY = 
	"CREATE TABLE IF NOT EXISTS " + SqliteDatabase::TABLE_USERS + " ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		"username TEXT NOT NULL UNIQUE, "
		"password TEXT NOT NULL, "
		"email TEXT NOT NULL"
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
	const int res = sqlite3_open(this->_dbName.c_str(), &(this->_dbInstance));
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

unsigned int SqliteDatabase::getIdOfUser(const std::string& username, const std::string& password) const
{
	const std::list<unsigned int> ids = queryIds(
		"SELECT id FROM " + TABLE_USERS +
		" WHERE "
		"username = '" + username + "'"
		" AND "
		"password = '" + password + "';"
	);

	if (ids.empty())
		return -1;

	return *ids.begin();
}

unsigned int SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& email) const
{
	return *queryIds(
		"INSERT INTO " + TABLE_USERS + " (username, password, email)"
		" VALUES "
		"('" + username + "','" + password + "','" + email + "')"

		" RETURNING id;"
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

std::list<unsigned int> SqliteDatabase::queryIds(const std::string &query) const
{
    return querySql<unsigned int>(
		query,

		[](const std::map<std::string, std::string> columns) -> unsigned int
		{
			return (unsigned int) std::stoul(columns.at("id"));
		}
	);
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
