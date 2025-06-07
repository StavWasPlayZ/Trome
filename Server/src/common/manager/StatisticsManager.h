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
    std::optional<UserStatistics> getUserStatistics(unsigned int id) const;

private:
	const IDatabase& m_database;
};