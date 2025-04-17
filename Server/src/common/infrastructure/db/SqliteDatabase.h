#pragma once

#include "IDatabase.h"
//REVIEW: Convert to SQLiteCpp
#include "sqlite3.h"

#include <string>
#include <map>
#include <list>
#include <functional>
#include <stdexcept> // for std::runtime_error

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
	unsigned int getIdOfUser(const std::string& username, const std::string& password) const override;

	unsigned int addNewUser(
		const std::string& username,
		const std::string& password,
		const std::string& email,
		const std::string& phone,
		const std::string& birthdate,
		const std::optional<std::string>& address
	) const override;

    void addTime(const std::string &username, const unsigned int time);
    void addTotalAns(const std::string &username, const unsigned int ans = 1);
    void addCorrectAns(const std::string &username, const unsigned int ans = 1);
    void addGamesPlayed(const std::string &username, const unsigned int games = 1);
    void addPoints(const std::string &username, const unsigned int points);

    unsigned int getTime(const std::string &username) const;
    unsigned int getTotalAns(const std::string &username) const;
    unsigned int getCorrectAns(const std::string &username) const;
    unsigned int getGamesPlayed(const std::string &username) const;
    unsigned int getPoints(const std::string &username) const;

    unsigned int getPlayerAverageAnsTime(const std::string &username) const;

private:
	SqliteDatabase(); 
	unsigned int getIdOfUser(const std::string &username) const;

	static const std::string CREATE_USERS_TBL_QUERY;
	static const std::string CREATE_STATISTICS_TBL_QUERY;
	static const std::string TABLE_USERS;
	static const std::string TABLE_STATISTICS;

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
		std::function<T(const std::map<std::string, std::string>&)> rowMapper
	) const;
	
	const std::string _dbName;
	sqlite3* _dbInstance;
};


#include "SqliteDatabase.tpp"
