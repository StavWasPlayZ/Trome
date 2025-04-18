# pragma once

#include "infrastructure/db/IDatabase.h"
#include <string>
#include <vector>
#include <utility> // for std::pair

class StatisticsManager
{
private:
	IDatabase *m_database;

public:
    StatisticsManager(IDatabase *db);
    ~StatisticsManager() = default;

    std::vector<std::pair<std::string, int>> getHighScore();
    std::vector<std::string &> &getUserStatistics(const std::string& username);

};