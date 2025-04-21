#pragma once

#include "SqliteDatabase.h"

template <typename T>
std::list<T> SqliteDatabase::querySql(const std::string &query,
                                      std::function<T(const std::map<std::string, std::string> &)> rowMapper) const
{
    std::list<T> results;

    consumeSql(
        query,

        [&results, &rowMapper](const std::map<std::string, std::string> &columns)
        {
            results.push_back(rowMapper(columns));
        }
    );

    return results;
}
