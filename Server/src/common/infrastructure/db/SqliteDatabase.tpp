#pragma once

#include "SqliteDatabase.h"

template <typename T>
std::list<T> SqliteDatabase::querySql(
    sqlite3_stmt *preppedStatement,
    const std::function<T(const std::map<std::string, std::optional<std::string>> &)> &columnMapper,
    const std::vector<std::string> &bindings
) const
{
    std::list<T> results;

    consumeSql(
        preppedStatement,

        [&results, &columnMapper](const std::map<std::string, std::optional<std::string>> &columns)
        {
            results.push_back(columnMapper(columns));
        },

        bindings
    );

    return results;
}
