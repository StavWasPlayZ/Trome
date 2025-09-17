#pragma once

#include "IDatabase.h"
//REVIEW: Convert to SQLiteCpp
#include "sqlite3.h"

#include <string>
#include <map>
#include <list>
#include <functional>

// for std::runtime_error (Windows):
// ReSharper disable once CppUnusedIncludeDirective
#include <stdexcept>

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase(const SqliteDatabase&) = delete;
	void operator=(const SqliteDatabase&) = delete;

	static SqliteDatabase& getInstance();

	~SqliteDatabase() override;

	bool open() override;
	bool close() override;
	bool doesUserExist(const std::string& username) const override;
	long queryIdOfUser(const std::string& username, const std::string& password) const override;

	long addNewUser(
		const std::string& username,
		const std::string& password,
		const std::string& email,
		const std::string& phone,
		const std::string& birthdate,
		const std::optional<std::string>& address
	) const override;


	int queryQuestionsCount() const override;
	std::list<Question> queryQuestions(int amount) const override;

	void addQuestions(std::vector<Question> questions, const std::optional<std::string>& authorName) const override;


    void addToStats(const std::string &username, int time, int answers, int correctAnswers, int points,
                    int games = 1) const override;

    std::map<UserModel, int> queryHighScores(int limit = 20) const override;

    std::optional<UserStatistics> getUserStatisticsById(long id) const override;

private:
	SqliteDatabase();

	static const std::string TABLE_USERS;
	static const std::string TABLE_STATISTICS;
	static const std::string TABLE_QUESTIONS;


	static const std::string CREATE_USERS_TBL_QUERY;
	static const std::string CREATE_STATISTICS_TBL_QUERY;
	static const std::string CREATE_QUESTIONS_TBL_QUERY;

    static const std::string USER_ID_QUERY;


    std::unordered_map<std::string, sqlite3_stmt*> _preppedStatements;

    void genPreppedStatements();


	long queryIdOfUser(const std::string &username) const;


	/**
	 * Simply executes the provided query.
	 */
	void execSql(sqlite3_stmt *preppedStatement, const std::vector<std::string> &bindings = {}) const;

    /**
     * Simply executes the provided query.
     */
    void execSql(const std::string& query, const std::vector<std::string> &bindings = {}) const;

	/*
	* Returns whether the exists query returns positive.
	* The associated column must be named "exists".
	*/
	bool queryExists(sqlite3_stmt *preppedStatement, const std::vector<std::string> &bindings = {}) const;
	/*
	* The associated column must be named "id".
	*/
	std::list<long> queryIds(sqlite3_stmt *preppedStatement, const std::vector<std::string> &bindings = {}) const;

    /**
     * Executes the provided query, returning as a list of integers,
     * as provided by the column named `colName`.
     */
	std::list<int> queryInts(
	    sqlite3_stmt *preppedStatement,
	    const std::string& colName,
	    const std::vector<std::string> &bindings = {}
	) const;


    /**
     * Executes the provided query, creating a temporary SQL statement.
     *
     * columnConsumer is passed, for each row found, a mapping of the column name to the column value (in string).
     */
    void consumeSql(
        const std::string &query,
        const std::function<void(const std::map<std::string, std::optional<std::string>> &)> &columnConsumer,
        const std::vector<std::string> &bindings = {}
    ) const;

    /**
     * Executes the provided query.
     *
     * columnConsumer is passed, for each row found, a mapping of the column name to the column value (in string).
     */
    void consumeSql(
        sqlite3_stmt *preppedStatement,
        const std::function<void(const std::map<std::string, std::optional<std::string>> &)> &columnConsumer,
        const std::vector<std::string> &bindings = {}
    ) const;

    static void bindPreparedStatement(sqlite3_stmt *preppedStatement, const std::vector<std::string> &bindings);

    sqlite3_stmt* genPreparedStatement(const std::string &query) const;

	/**
	 * Executes the provided query.
	 *
	 * columnMapper is passed, for each row found, a mapping of the column name to the column value (in string).
	 *
	 * The result returned from said method will be accumulated as an item in the overall returned list.
	 */
	template <typename T>
	std::list<T> querySql(
		sqlite3_stmt *preppedStatement,
		const std::function<T(const std::map<std::string, std::optional<std::string>>&)> &columnMapper,
		const std::vector<std::string> &bindings = {}
	) const;

	const std::string _dbName;
	sqlite3* _dbInstance;
};


#include "SqliteDatabase.tpp"
