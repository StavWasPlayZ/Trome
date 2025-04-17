#pragma once

#include "IDatabase.h"
//REVIEW: Convert to SQLiteCpp
#include "sqlite3.h"

#include <string>
#include <map>
#include <list>
#include <functional>

//REVIEW: This is already defined in the CPP.
// Does it make any issues still...?
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

    void addTime(const std::string &username, int time) override;
    void addTotalAns(const std::string &username, int ans = 1) override;
    void addCorrectAns(const std::string &username, int ans = 1) override;
    void addGamesPlayed(const std::string &username, int games = 1) override;
    void addPoints(const std::string &username, int points) override;

    int getTime(const std::string &username) const override;
    int getTotalAns(const std::string &username) const override;
    int getCorrectAns(const std::string &username) const override;
    int getGamesPlayed(const std::string &username) const override;
    int getPoints(const std::string &username) const override;

    float getPlayerAverageAnsTime(const std::string &username) const override;

protected:
	unsigned int getIdOfUser(const std::string &username) const override;
    void addToColumn(const std::string &username, const std::string &column, int n, const std::string &table) override;

private:
	SqliteDatabase();

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
     * Executes the provided query, returning as a list of integers,
     * as provided by the column named `colName`.
     */
	std::list<int> queryInts(const std::string& query, const std::string& colName) const;

	int getStat(const std::string &username, const std::string &colName) const;


	static std::string genQueryUserIdStr(const std::string& username);


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

	/**
	 * Either returns the first element in the provided list, or -1 if none.
	 */
	template <typename T>
	static const T& getResultAsSingular(const std::list<T>& results);

	const std::string _dbName;
	sqlite3* _dbInstance;
};


#include "SqliteDatabase.tpp"
