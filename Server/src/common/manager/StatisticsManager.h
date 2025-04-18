# pragma once

#include "infrastructure/db/IDatabase.h"
#include <string>
#include <vector>
#include <utility> // for std::pair

class StatisticsManager
{
public:
    explicit StatisticsManager(const IDatabase& db);

    std::vector<std::pair<std::string, int>> getHighScore() const;
    std::vector<std::string> getUserStatistics(const std::string& username) const;

private:
	const IDatabase& m_database;
};