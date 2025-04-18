# pragma once

#include "infrastructure/db/IDatabase.h"
#include <string>
#include <vector>

class StatisticsManager
{
private:
	IDatabase *m_database;

public:
    StatisticsManager(const IDatabase *db);
    ~StatisticsManager() = default;

    std::vector<std::string &> &getHighScore();
    std::vector<std::string &> &getUserStatistics(const std::string& username);

};