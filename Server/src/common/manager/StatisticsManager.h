# pragma once

#include "infrastructure/UserStatistics.h"
#include "infrastructure/db/IDatabase.h"
#include "infrastructure/model/UserModel.h"

#include <string>
#include <utility>
#include <vector>

class StatisticsManager
{
public:
    explicit StatisticsManager(const IDatabase& db);

    std::vector<std::pair<UserModel, int>> getHighScores() const;
    UserStatistics getUserStatistics(const std::string& username) const;

private:
	const IDatabase& m_database;
};