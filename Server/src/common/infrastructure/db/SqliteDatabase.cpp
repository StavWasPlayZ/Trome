#include "SqliteDatabase.h"

#include <sstream>
#include <iostream>

const std::string SqliteDatabase::TABLE_USERS = "users";
const std::string SqliteDatabase::TABLE_STATISTICS = "statistics";

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

const std::string SqliteDatabase::CREATE_STATISTICS_TBL_QUERY = 
	"CREATE TABLE IF NOT EXISTS " + TABLE_STATISTICS + " ("
        "user_id INT PRIMARY KEY, "
        "total_time INT NOT NULL, "
        "correct_ans INT NOT NULL, "
        "total_ans INT NOT NULL, "
        "games_played INT NOT NULL, "
        "points INT NOT NULL, "
	    "FOREIGN KEY(user_id) REFERENCES " + TABLE_USERS + "(id)"
    ");";



SqliteDatabase::SqliteDatabase() : _dbName("trivia-database.db"), _dbInstance(nullptr)
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
    execSql(CREATE_STATISTICS_TBL_QUERY);

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
	return getResultAsSingular(queryIds(
		"SELECT id FROM " + TABLE_USERS +
		" WHERE "
		"username = '" + username + "'"
		" AND "
		"password = '" + password + "';"
	));
}

unsigned int SqliteDatabase::getIdOfUser(const std::string &username) const
{
    return getResultAsSingular(queryIds(
        genQueryUserIdStr(username) + ";"
    ));
}

void SqliteDatabase::addToColumn(const std::string &username, const std::string &column, int n,
                                 const std::string &table)
{
    std::ostringstream builder;

    builder << "UPDATE " << table <<
        " SET " << column << " = " << column << " + " << n <<
        " WHERE "
        "user_id = (" << genQueryUserIdStr(username) << ")"
    ";";

    execSql(builder.str());
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

void SqliteDatabase::addTime(const std::string &username, const int time)
{
	addToColumn(username, "total_time", time, TABLE_STATISTICS);
}

void SqliteDatabase::addTotalAns(const std::string &username, const int ans)
{
    addToColumn(username, "total_ans", ans, TABLE_STATISTICS);
}

void SqliteDatabase::addCorrectAns(const std::string &username, const int ans)
{
    addToColumn(username, "correct_ans", ans, TABLE_STATISTICS);
}

void SqliteDatabase::addGamesPlayed(const std::string &username, const int games)
{
    addToColumn(username, "games_played", games, TABLE_STATISTICS);
}

void SqliteDatabase::addPoints(const std::string &username, const int points)
{
    addToColumn(username, "points", points, TABLE_STATISTICS);
}

int SqliteDatabase::getTime(const std::string &username) const
{
    return getStat(username, "total_time");
}

int SqliteDatabase::getTotalAns(const std::string &username) const
{
    return getStat(username, "total_ans");
}

int SqliteDatabase::getCorrectAns(const std::string &username) const
{
    return getStat(username, "correct_ans");
}

int SqliteDatabase::getGamesPlayed(const std::string &username) const
{
    return getStat(username, "games_played");
}

int SqliteDatabase::getPoints(const std::string &username) const
{
    return getStat(username, "points");
}

float SqliteDatabase::getPlayerAverageAnsTime(const std::string &username) const
{
    const unsigned int totalTime = getTime(username);
    const unsigned int totalAns = getTotalAns(username);

    if (totalAns == -1 || totalAns == 0 || totalTime == -1)
    {
        return -1;
    }

    return (float)totalTime / totalAns;
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

std::list<int> SqliteDatabase::queryInts(const std::string& query, const std::string& colName) const
{
    return querySql<int>(
        query,

        [colName](const std::map<std::string, std::string> &columns) -> int
        {
            return std::stoi(columns.at(colName));
        }
    );
}

int SqliteDatabase::getStat(const std::string &username, const std::string &colName) const
{
    std::ostringstream builder;

    builder << "SELECT " << colName << " FROM " << TABLE_STATISTICS <<
        " WHERE "
        "user_id = (" << genQueryUserIdStr(username) << ")"
    ";";

    return getResultAsSingular(queryInts(builder.str(), colName));
}

std::string SqliteDatabase::genQueryUserIdStr(const std::string& username)
{
    return "SELECT id FROM " + TABLE_USERS +
        " WHERE "
        "username = '" + username + "'"
    ;
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
