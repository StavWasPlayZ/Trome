#pragma once

#include "SqliteDatabase.h"


// Because SQLite cries and sobs when we pass captures to a lambda:
template <typename T>
struct QueryCallbackContext
{
	std::list<T>& results;
	std::function<T(const std::map<std::string, std::string>&)>& rowMapper;
};

template <typename T>
std::list<T> SqliteDatabase::querySql(
	const std::string& query,
	std::function<T(const std::map<std::string, std::string>&)> rowMapper
) const
{
	char* errMsg;
	std::list<T> results;

	QueryCallbackContext<T> context = {
		results,
		rowMapper
	};

	const int result = sqlite3_exec(
		this->_dbInstance,
		query.c_str(),
		[](void* data, const int argc, char** argv, char** azColName) -> int
		{
			const QueryCallbackContext<T>* context = (QueryCallbackContext<T>*) data;

			// Convert the args to a string vector to be passed to the provided mapper function
			std::map<std::string, std::string> columns;

			for (size_t i = 0; i < argc; i++)
			{
				columns[std::string(azColName[i])] = std::string(argv[i]);
			}

			context->results.push_back(context->rowMapper(columns));

			return 0;
		},
		&context,
		&errMsg
	);

	if (result != SQLITE_OK)
	{
		throw std::runtime_error("Error in SQL: " + std::string(errMsg));
	}

	return results;
}
