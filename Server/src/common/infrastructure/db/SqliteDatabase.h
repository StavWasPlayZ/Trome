#pragma once

#include "IDatabase.h"
//REVIEW: Convert to SQLiteCpp
#include "sqlite3.h"

#include <string>
#include <map>
#include <list>
#include <functional>

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase();
	virtual ~SqliteDatabase();

	virtual bool open() override;
	virtual bool close() override;
	virtual bool doesUserExist(const std::string& username) const override;
	virtual unsigned int getIdOfUser(const std::string& username, const std::string& password) const override;
	virtual unsigned int addNewUser(const std::string& username, const std::string& password, const std::string& email) const override;

private:
	static const std::string CREATE_USERS_TBL_QUERY;
	static const std::string TABLE_USERS;

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
