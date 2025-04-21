#include "SqliteDatabase.h"

#include <sstream>
#include <iostream>

const std::string SqliteDatabase::TABLE_USERS = "users";
const std::string SqliteDatabase::TABLE_STATISTICS = "statistics";
const std::string SqliteDatabase::TABLE_QUESTIONS = "questions";

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
		// but shall be entertained for as a string for the sake of the exercise.
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

const std::string SqliteDatabase::CREATE_QUESTIONS_TBL_QUERY =
    "CREATE TABLE IF NOT EXISTS " + TABLE_QUESTIONS + " ("
        "id INTEGER PRIMARY KEY NOT NULL, "
        "question NVARCHAR(120) NOT NULL, "
        "correct NVARCHAR(64) NOT NULL, "
        "wrong_1 NVARCHAR(64) NOT NULL, "
        "wrong_2 NVARCHAR(64) NOT NULL, "
        "wrong_3 NVARCHAR(64) NOT NULL"
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
    execSql(CREATE_QUESTIONS_TBL_QUERY);

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

unsigned int SqliteDatabase::queryIdOfUser(const std::string& username, const std::string& password) const
{
    const std::list<unsigned int> results = queryIds(
        "SELECT id FROM " + TABLE_USERS +
        " WHERE "
        "username = '" + username + "'"
        " AND "
        "password = '" + password + "';"
    );

	if (results.empty())
	{
	    return -1;
	}

    return *results.begin();
}

unsigned int SqliteDatabase::queryIdOfUser(const std::string &username) const
{
    const std::list<unsigned int> results = queryIds(
        genQueryUserIdStr(username) + ";"
    );

    if (results.empty())
    {
        return -1;
    }

    return *results.begin();
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

unsigned int SqliteDatabase::addNewUser(const std::string &username, const std::string &password,
                                        const std::string &email, const std::string &phone,
                                        const std::string &birthdate, const std::optional<std::string> &address) const
{
    validateSignupInfo(password, email, phone, birthdate, address);

    std::ostringstream builderUsers;
    std::ostringstream builderStats;
    int id = 0;

    builderUsers << "INSERT INTO " << TABLE_USERS
        << " (username, password, email, phone, address, birthdate)"
        << " VALUES "
        << "('"
            << username << "','" << password << "','" << email << "','" << phone << "','"
            << (address.has_value() ? address.value() : "NULL") << "','" << birthdate
        << "')"
        << " RETURNING id;";

    id = *queryIds(builderUsers.str()).begin();

    builderStats << "INSERT INTO " << TABLE_STATISTICS
        << "(user_id, total_time, correct_ans, total_ans, games_played, points)"
        << " VALUES "
        << "(" << id << ", 0, 0, 0, 0, 0)"
        << ";";

    execSql(builderStats.str());

    return id;
}

int SqliteDatabase::queryQuestionsCount() const
{
    return *queryInts(
        "SELECT COUNT(*) AS count FROM " + TABLE_QUESTIONS,
        "count"
    ).begin();
}

std::list<Question> SqliteDatabase::queryQuestions(const int amount) const
{
    return querySql<Question>(
        "SELECT * FROM " + TABLE_QUESTIONS +
        " ORDER BY RANDOM()"
        " LIMIT " + std::to_string(amount) +
        ";",

        [](const std::map<std::string, std::string> &columns) -> Question
        {
            std::vector<std::string> answers;
            answers.reserve(Question::QUESTIONS_AMOUNT);

            answers.push_back(columns.at("correct"));

            for (int i = 1; i < Question::QUESTIONS_AMOUNT; i++)
            {
                answers.push_back(columns.at("wrong_" + std::to_string(i)));
            }

            return Question(columns.at("question"), answers);
        }
    );
}

void SqliteDatabase::addQuestions(std::vector<Question> questions) const
{
    std::ostringstream builder;

    builder << "INSERT INTO " << TABLE_QUESTIONS << " (question, correct, wrong_1, wrong_2, wrong_3)"
        " VALUES ";

    bool first = true;

    for (const Question &question : questions)
    {
        if (!first)
        {
            builder << ", ";
        }
        first = false;


        builder << "("
            << question.question;

        for (const std::string &answer : question.answers)
        {
            builder << ", " << answer;
        }

        builder << ")";
    }

    execSql(builder.str());
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

int SqliteDatabase::queryTime(const std::string &username) const
{
    return queryStat(username, "total_time");
}

int SqliteDatabase::queryTotalAns(const std::string &username) const
{
    return queryStat(username, "total_ans");
}

int SqliteDatabase::queryCorrectAns(const std::string &username) const
{
    return queryStat(username, "correct_ans");
}

int SqliteDatabase::queryGamesPlayed(const std::string &username) const
{
    return queryStat(username, "games_played");
}

int SqliteDatabase::queryPoints(const std::string &username) const
{
    return queryStat(username, "points");
}

float SqliteDatabase::queryPlayerAverageAnsTime(const std::string &username) const
{
    const int totalTime = queryTime(username);
    const int totalAns = queryTotalAns(username);

    if (totalAns == -1 || totalAns == 0 || totalTime == -1)
    {
        return -1;
    }

    return static_cast<float>(totalTime) / totalAns;
}

std::unordered_map<std::string, int> SqliteDatabase::queryHighScores(const int limit) const
{
    std::ostringstream builder;
    builder << "SELECT users.username, stats.points "
        << "FROM " << TABLE_USERS << " users "
        << "JOIN " << TABLE_STATISTICS << " stats "
        << "ON users.id = stats.user_id "
        << "ORDER BY stats.points DESC "
        << "LIMIT " << limit <<
    ";";

    std::unordered_map<std::string, int> results;

    consumeSql(
        builder.str(),

        [&results](const std::map<std::string, std::string> &row)
        {
            results.emplace(row.at("username"), std::stoi(row.at("points")));
        }
    );

	return results;
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
			return static_cast<unsigned int>(std::stoul(columns.at("id")));
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

int SqliteDatabase::queryStat(const std::string &username, const std::string &colName) const
{
    std::ostringstream builder;

    builder << "SELECT " << colName << " FROM " << TABLE_STATISTICS <<
        " WHERE "
        "user_id = (" << genQueryUserIdStr(username) << ")"
    ";";

    const std::list<int> results = queryInts(builder.str(), colName);

    if (results.empty())
    {
        return -1;
    }

    return *results.begin();
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

void SqliteDatabase::consumeSql(
    const std::string &query,
    std::function<void(const std::map<std::string, std::string> &)> rowConsumer
) const {
    char *errMsg;

    const int result = sqlite3_exec(
        this->_dbInstance, query.c_str(),
        [](void *data, const int argc, char **argv, char **azColName) -> int {
            const std::function<void(const std::map<std::string, std::string> &)> rowConsumer =
                *static_cast<std::function<void(const std::map<std::string, std::string> &)>*>(data);

            // Convert the args to a string vector to be passed to the provided mapper function
            std::map<std::string, std::string> columns;

            for (size_t i = 0; i < argc; i++)
            {
                columns[std::string(azColName[i])] = std::string(argv[i]);
            }

            rowConsumer(columns);
            return 0;
        },

        &rowConsumer,
        &errMsg
    );

    if (result != SQLITE_OK)
    {
        const std::string msg = std::string(errMsg);
        sqlite3_free(errMsg);

        throw std::runtime_error("Error in SQL: " + msg);
    }
}
