#include "SqliteDatabase.h"

#include <sstream>
#include <iostream>
#include <stdexcept>

const std::string SqliteDatabase::TABLE_USERS = "users";

const std::string SqliteDatabase::CREATE_USERS_TBL_QUERY = 
	"CREATE TABLE IF NOT EXISTS " + TABLE_USERS + " ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT, "
		// Lengths below are as suggested by the internet
		"username NVARCHAR(20) NOT NULL UNIQUE, "
		"password NVARCHAR(64) NOT NULL, "
		"email NVARCHAR(254) NOT NULL, "

		"phone NVARCHAR(16) NOT NULL, "
		"address NVARCHAR(180), "
		// Would 99% of times be of type DATE,
		// but shall be entertained as a string for the sake of the exercise.
		// DD/MM/YYYY
		"birthdate NVARCHAR(10) NOT NULL"
	");";


SqliteDatabase::SqliteDatabase() : _dbName("trivia-database"), _dbInstance(nullptr)
{
    std::cout << "C++ SQLite version: " << sqlite3_libversion() << std::endl;
}

SqliteDatabase &SqliteDatabase::getInstance()
{
    static SqliteDatabase instance;
    return instance;
}

SqliteDatabase::~SqliteDatabase()
{
	SqliteDatabase::close();
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

unsigned int SqliteDatabase::addNewUser(
	const std::string& username,
	const std::string& password,
	const std::string& email,
	const std::string& phone,
	const std::string& birthdate,
	const std::optional<std::string>& address
) const {
    validateSignupInfo(password, email, phone, birthdate, address);

	std::ostringstream builder;

	builder << "INSERT INTO " << TABLE_USERS << " (username, password, email, phone, address, birthdate)"
		" VALUES "
		"('"
			<< username << "','"
			<< password << "','"
			<< email << "','"
			<< phone << "','"
			<< (address.has_value() ? address.value() : "NULL") << "','"
			<< birthdate <<
		"')"
	" RETURNING id;";

	return *queryIds(builder.str()).begin();
}



// Generic wrapper implementations

bool SqliteDatabase::queryExists(const std::string& query) const
{
	return *querySql<bool>(
		query,
		[](const std::map<std::string, std::string> &columns) -> bool
		{
			return columns.at("q_exists") == "1";
		}
	).begin();
}

std::list<unsigned int> SqliteDatabase::queryIds(const std::string &query) const
{
    return querySql<unsigned int>(
		query,

		[](const std::map<std::string, std::string> &columns) -> unsigned int
		{
			return (unsigned int) std::stoul(columns.at("id"));
		}
	);
}

void SqliteDatabase::execSql(const std::string& query) const
{
	char* errMessage;

	const int result = sqlite3_exec(
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
