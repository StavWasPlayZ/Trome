# pragma once

#include "infrastructure/db/IDatabase.h"
#include "infrastructure/UserStatistics.h"

#include <string>
#include <utility>
#include <vector>

class LoggedUser;

class StatisticsManager
{
public:
    explicit StatisticsManager(const IDatabase& db);

    std::vector<std::pair<LoggedUser*, int>> getHighScores() const;
    UserStatistics getUserStatistics(const std::string& username) const;

private:
	const IDatabase& m_database;
};