#include "SqliteDatabase.h"

#include "infrastructure/model/UserModel.h"

#include <iostream>
#include <sstream>

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
        "author_id INT, "

        "question NVARCHAR(120) UNIQUE NOT NULL, "
        "correct NVARCHAR(64) NOT NULL, "
        "wrong_1 NVARCHAR(64) NOT NULL, "
        "wrong_2 NVARCHAR(64) NOT NULL, "
        "wrong_3 NVARCHAR(64) NOT NULL, "
        "FOREIGN KEY(author_id) REFERENCES " + TABLE_USERS + "(id)"
    ");";


const std::string SqliteDatabase::USER_ID_QUERY =
    "SELECT id FROM " + TABLE_USERS +
    " WHERE "
    "username = ?";


void SqliteDatabase::genPreppedStatements()
{
    _preppedStatements = {
        {"doesUserExist", genPreparedStatement(
            "SELECT EXISTS("
                "SELECT 1 FROM " + TABLE_USERS +
                " WHERE"
                    " username = ?"
            ") AS q_exists;"
        )},

        {"doesUserExist", genPreparedStatement(
            "SELECT id FROM " + TABLE_USERS +
            " WHERE"
                " username = ?"
                " AND"
                " password = ?"
            ";"
        )},

        {"queryIdOfUser", genPreparedStatement(
            USER_ID_QUERY + ";"
        )},

        {"addNewUser", genPreparedStatement(
            "INSERT INTO " + TABLE_USERS +
                " (username, password, email, phone, address, birthdate)"
            " VALUES"
                " (?, ?, ?, ?, ?, ?)"
            " RETURNING id;"
        )},

        {"insertEmptyStats", genPreparedStatement(
            "INSERT INTO " + TABLE_STATISTICS +
                " (user_id, total_time, correct_ans, total_ans, games_played, points)"
            " VALUES"
                " (?, 0, 0, 0, 0, 0)"
            ";"
        )},

        {"queryQuestionsCount", genPreparedStatement(
            "SELECT COUNT(*) AS count FROM " + TABLE_QUESTIONS
        )},

        {"queryQuestions", genPreparedStatement(
            "SELECT * FROM " + TABLE_QUESTIONS +
            " ORDER BY RANDOM()"
            " LIMIT ?"
            ";"
        )},

        {"addToStats", genPreparedStatement(
            "INSERT INTO " + TABLE_STATISTICS +
                " (user_id, total_time, correct_ans, total_ans, games_played, points)"
            " VALUES ((" + USER_ID_QUERY + "), ?, ?, ?, ?, ?)"
            " ON CONFLICT(user_id) DO UPDATE SET"
                " total_time = total_time + excluded.total_time,"
                " correct_ans = correct_ans + excluded.correct_ans,"
                " total_ans = total_ans + excluded.total_ans,"
                " games_played = games_played + excluded.games_played,"
                " points = points + excluded.points"
            ";"
        )},

        {"queryHighScores", genPreparedStatement(
            "SELECT users.id, users.username, stats.points"
                " FROM " + TABLE_USERS + " users"
                " JOIN " + TABLE_STATISTICS + " stats"
                    " ON users.id = stats.user_id"
                " ORDER BY stats.points DESC"
                " LIMIT ?"
            ";"
        )},

        {"getUserStatisticsById", genPreparedStatement(
            "SELECT * FROM " + TABLE_STATISTICS +
                " WHERE user_id = ?"
            ";"
        )}
    };
}


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

    genPreppedStatements();
	return true;
}

bool SqliteDatabase::close()
{
	if (sqlite3_close(this->_dbInstance) == SQLITE_OK)
	{
		this->_dbInstance = nullptr;
		return true;
	}

    // Free all prepared statements
    for (const auto &pair : this->_preppedStatements)
    {
        sqlite3_finalize(pair.second);
    }

    this->_preppedStatements.clear();
	return false;
}

bool SqliteDatabase::doesUserExist(const std::string& username) const
{
    return queryExists(
        this->_preppedStatements.at("doesUserExist"),
        {username}
    );
}

unsigned int SqliteDatabase::queryIdOfUser(const std::string& username, const std::string& password) const
{
    const std::list<unsigned int> results = queryIds(
        this->_preppedStatements.at("queryIdOfUser"),
        {username, password}
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
        this->_preppedStatements.at("queryIdOfUser"),
        {username}
    );

    if (results.empty())
    {
        return -1;
    }

    return *results.begin();
}

unsigned int SqliteDatabase::addNewUser(const std::string &username, const std::string &password,
                                        const std::string &email, const std::string &phone,
                                        const std::string &birthdate, const std::optional<std::string> &address) const
{
    validateSignupInfo(password, email, phone, birthdate, address);

    const int id = *queryIds(
        this->_preppedStatements.at("addNewUser"),
        {
            username,
            password,
            email,
            phone,
            address.has_value() ? address.value() : "NULL",
            birthdate
        }
    ).begin();

    execSql(
        this->_preppedStatements.at("insertEmptyStats"),
        {std::to_string(id)}
    );

    return id;
}

int SqliteDatabase::queryQuestionsCount() const
{
    return *queryInts(
        this->_preppedStatements.at("queryQuestionsCount"),
        "count"
    ).begin();
}

std::list<Question> SqliteDatabase::queryQuestions(const int amount) const
{
    return querySql<Question>(
        this->_preppedStatements.at("queryQuestions"),

        [](const std::map<std::string, std::optional<std::string>> &columns) -> Question
        {
            std::vector<std::string> answers;
            answers.reserve(Question::QUESTIONS_AMOUNT);

            answers.push_back(columns.at("correct").value());

            for (int i = 1; i < Question::QUESTIONS_AMOUNT; i++)
            {
                answers.push_back(columns.at("wrong_" + std::to_string(i)).value());
            }

            return Question(columns.at("question").value(), answers);
        },

        {std::to_string(amount)}
    );
}

void SqliteDatabase::addQuestions(const std::vector<Question> questions,
                                  const std::optional<std::string> &authorName) const
{
    const std::string authorId = authorName.has_value() ? std::to_string(queryIdOfUser(*authorName)) : "NULL";

    std::ostringstream builder;

    builder << "INSERT INTO " << TABLE_QUESTIONS
            << " (author_id, question, correct, wrong_1, wrong_2, wrong_3)"
               " VALUES ";

    std::vector<std::string> bindings;

    for (size_t i = 0; i < questions.size(); ++i)
    {
        if (i > 0)
        {
            builder << ", ";
        }

        builder << "(?, ?, ?, ?, ?, ?)";

        const Question &question = questions.at(i);

        bindings.push_back(authorId);
        bindings.push_back(question.prompt);

        for (const std::string &answer : question.answers)
        {
            bindings.push_back(answer);
        }
    }

    builder << ";";

    execSql(builder.str(), bindings);
}

void SqliteDatabase::addToStats(const std::string &username, const int time, const int answers,
                                const int correctAnswers, const int points, const int games) const
{
    execSql(
        this->_preppedStatements.at("addToStats"),
        {
            username,
            std::to_string(time),
            std::to_string(correctAnswers),
            std::to_string(answers),
            std::to_string(games),
            std::to_string(points)
        }
    );
}

std::map<UserModel, int> SqliteDatabase::queryHighScores(const int limit) const
{
    std::map<UserModel, int> results;

    consumeSql(
        this->_preppedStatements.at("queryHighScores"),

        [&results](const std::map<std::string, std::optional<std::string>> &row)
        {
            results.emplace(
                UserModel(
                    std::stoi(row.at("id").value()),
                    row.at("username").value()
                ),
                std::stoi(row.at("points").value())
            );
        },

        {std::to_string(limit)}
    );

	return results;
}

std::optional<UserStatistics> SqliteDatabase::getUserStatisticsById(const unsigned int id) const
{
    std::list<UserStatistics> results = querySql<UserStatistics>(
        this->_preppedStatements.at("getUserStatisticsById"),

        [](const std::map<std::string, std::optional<std::string>> &row) -> UserStatistics
        {
            const int totalTime = std::stoi(row.at("total_time").value());
            const int totalAns = std::stoi(row.at("total_ans").value());

            return UserStatistics(
                std::stoi(row.at("points").value()),
                std::stoi(row.at("games_played").value()),
                totalAns,
                std::stoi(row.at("correct_ans").value()),
                totalTime,
                calcAverageAnswerTime(totalTime, totalAns)
            );
        },

        {std::to_string(id)}
    );

    if (results.empty())
    {
        return std::nullopt;
    }

    return *results.begin();
}


// Generic wrapper implementations

bool SqliteDatabase::queryExists(sqlite3_stmt *preppedStatement, const std::vector<std::string> &bindings) const
{
	return *querySql<bool>(
		preppedStatement,
		[](const std::map<std::string, std::optional<std::string>> &columns) -> bool
		{
			return columns.at("q_exists").value() == "1";
		},

		bindings
	).begin();
}

std::list<unsigned int> SqliteDatabase::queryIds(sqlite3_stmt *preppedStatement,
                                                 const std::vector<std::string> &bindings) const
{
    return querySql<unsigned int>(
		preppedStatement,

		[](const std::map<std::string, std::optional<std::string>> &columns) -> unsigned int
		{
			return static_cast<unsigned int>(std::stoul(columns.at("id").value()));
		},

		bindings
	);
}

std::list<int> SqliteDatabase::queryInts(sqlite3_stmt *preppedStatement, const std::string& colName,
                                         const std::vector<std::string> &bindings) const
{
    return querySql<int>(
        preppedStatement,

        [colName](const std::map<std::string, std::optional<std::string>> &columns) -> int
        {
            return std::stoi(columns.at(colName).value());
        },

        bindings
    );
}

void SqliteDatabase::execSql(sqlite3_stmt *preppedStatement, const std::vector<std::string> &bindings) const
{
    bindPreparedStatement(preppedStatement, bindings);

    const int result = sqlite3_step(preppedStatement);

    if (result != SQLITE_DONE)
    {
        const std::string msg = sqlite3_errmsg(this->_dbInstance);
        sqlite3_reset(preppedStatement);
        sqlite3_clear_bindings(preppedStatement);

        throw std::runtime_error("Error in SQL: " + msg);
    }

    sqlite3_reset(preppedStatement);
    sqlite3_clear_bindings(preppedStatement);
}

void SqliteDatabase::execSql(const std::string& query, const std::vector<std::string> &bindings) const
{
    sqlite3_stmt *const preppedStatement = genPreparedStatement(query);

    try
    {
        execSql(preppedStatement, bindings);
    }
    catch (const std::exception &)
    {
        sqlite3_finalize(preppedStatement);
        throw;
    }

    sqlite3_finalize(preppedStatement);
}

void SqliteDatabase::consumeSql(
    const std::string &query,
    const std::function<void(const std::map<std::string, std::optional<std::string>> &)> &columnConsumer,
    const std::vector<std::string> &bindings
) const
{
    sqlite3_stmt *const preppedStatement = genPreparedStatement(query);

    try
    {
        consumeSql(preppedStatement, columnConsumer, bindings);
    }
    catch (const std::exception &)
    {
        sqlite3_finalize(preppedStatement);
        throw;
    }

    sqlite3_finalize(preppedStatement);
}

void SqliteDatabase::consumeSql(
    sqlite3_stmt *const preppedStatement,
    const std::function<void(const std::map<std::string, std::optional<std::string>> &)> &columnConsumer,
    const std::vector<std::string> &bindings
) const
{
    bindPreparedStatement(preppedStatement, bindings);

    const int columnsCount = sqlite3_column_count(preppedStatement);

    int result;
    while ((result = sqlite3_step(preppedStatement)) == SQLITE_ROW)
    {
        // Convert the results to a map of column name to column value.
        // This will be passed to the consumer function.
        std::map<std::string, std::optional<std::string>> columns;

        for (int i = 0; i < columnsCount; i++)
        {
            const char *columnName = sqlite3_column_name(preppedStatement, i);
            const unsigned char *columnValue = sqlite3_column_text(preppedStatement, i);

            columns.emplace(
                columnName,
                columnValue != nullptr
                    ? std::optional(reinterpret_cast<const char *>(columnValue))
                    : std::nullopt
            );
        }

        columnConsumer(columns);
    }

    if (result != SQLITE_DONE)
    {
        const std::string msg = sqlite3_errmsg(this->_dbInstance);
        sqlite3_reset(preppedStatement);
        sqlite3_clear_bindings(preppedStatement);

        throw std::runtime_error("Error in SQL - Unexpected ending of rows stream: " + msg);
    }

    sqlite3_reset(preppedStatement);
    sqlite3_clear_bindings(preppedStatement);
}

void SqliteDatabase::bindPreparedStatement(sqlite3_stmt *preppedStatement,
                                           const std::vector<std::string> &bindings)
{
    for (size_t i = 0; i < bindings.size(); i++)
    {
        // Just bind for texts, no matter the binding type.
        //
        // Notice that we've made the bindings list of strings and not "objects".
        // If it were to be a typed OOP language or something then we could check instanceof/is/etc.
        // Matter of development comfort only.

        const std::string& binding = bindings.at(i);
        sqlite3_bind_text(preppedStatement, i + 1, binding.c_str(), binding.length(), SQLITE_STATIC);
    }
}

sqlite3_stmt *SqliteDatabase::genPreparedStatement(const std::string &query) const
{
    sqlite3_stmt *preppedStatement;
    const int result = sqlite3_prepare_v2(
        this->_dbInstance,
        query.c_str(),
        query.length(),
        &preppedStatement,
        nullptr
    );

    if (result != SQLITE_OK)
    {
        const std::string msg = sqlite3_errmsg(this->_dbInstance);
        throw std::runtime_error("Error in SQL - Failed to prepare: " + msg);
    }

    return preppedStatement;
}
