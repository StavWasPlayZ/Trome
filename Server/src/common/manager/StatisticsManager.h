# pragma once

#include "infrastructure/db/IDatabase.h"
#include "infrastructure/UserStatistics.h"

#include <string>
#include <vector>
#include <utility>

class StatisticsManager
{
public:
    explicit StatisticsManager(const IDatabase& db);

    std::vector<std::pair<std::string, int>> getHighScores() const;
    UserStatistics getUserStatistics(const std::string& username) const;

private:
	const IDatabase& m_database;
};