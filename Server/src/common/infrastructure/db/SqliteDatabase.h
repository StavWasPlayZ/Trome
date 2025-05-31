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
	unsigned int queryIdOfUser(const std::string& username, const std::string& password) const override;

	unsigned int addNewUser(
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


    void addTime(const std::string &username, int time) const override;
    void addTotalAns(const std::string &username, int ans = 1) const override;
    void addCorrectAns(const std::string &username, int ans = 1) const override;
    void addGamesPlayed(const std::string &username, int games = 1) const override;
    void addPoints(const std::string &username, int points) const override;

    int queryTime(const std::string &username) const override;
    int queryTotalAns(const std::string &username) const override;
    int queryCorrectAns(const std::string &username) const override;
    int queryGamesPlayed(const std::string &username) const override;
    int queryPoints(const std::string &username) const override;

    float queryPlayerAverageAnsTime(const std::string &username) const override;

    std::unordered_map<std::string, int> queryHighScores(int limit = 20) const override;

protected:
	unsigned int queryIdOfUser(const std::string &username) const override;
    void addToColumn(const std::string &username, const std::string &column, int n, const std::string &table) const override;

private:
	SqliteDatabase();

	static const std::string TABLE_USERS;
	static const std::string TABLE_STATISTICS;
	static const std::string TABLE_QUESTIONS;

	static const std::string CREATE_USERS_TBL_QUERY;
	static const std::string CREATE_STATISTICS_TBL_QUERY;
	static const std::string CREATE_QUESTIONS_TBL_QUERY;

	/**
	 * Simply executes the provided query.
	 */
	void execSql(const std::string& query) const;

	/*
	* Returns whether the exists query returns positive.
	* The associated column must be named "exists".
	*/
	bool queryExists(const std::string& query) const;
	/*
	* The associated column must be named "id".
	*/
	std::list<unsigned int> queryIds(const std::string& query) const;

    /**
     * Executes the provided query, returning as a list of integers,
     * as provided by the column named `colName`.
     */
	std::list<int> queryInts(const std::string& query, const std::string& colName) const;

	int queryStat(const std::string &username, const std::string &colName) const;


	static std::string genQueryUserIdStr(const std::string& username);


	/**
	 * Executes the provided query.
	 *
	 * rowConsumer is passed, for each row found, a mapping of
	 * the column name to the value (in string) that it possesses.
	 */
	void consumeSql(
		const std::string& query,
		std::function<void(const std::map<std::string, std::optional<std::string>>&)> rowConsumer
	) const;

	/**
	 * Executes the provided query.
	 *
	 * rowMapper is passed, for each row found, a mapping of
	 * the column name to the value (in string) that it possesses.
	 *
	 * The result returned from it will be accumulated as an item in the returned list.
	 */
	template <typename T>
	std::list<T> querySql(
		const std::string& query,
		std::function<T(const std::map<std::string, std::optional<std::string>>&)> rowMapper
	) const;

	const std::string _dbName;
	sqlite3* _dbInstance;
};


#include "SqliteDatabase.tpp"
