#include "StatisticsManager.h"

#include "infrastructure/Server.h"

#include <algorithm>

StatisticsManager::StatisticsManager(const IDatabase& db) :
    m_database(db)
{}

std::vector<std::pair<UserModel, int>> StatisticsManager::getHighScores() const
{
    //TODO: Cache this. Only update once per sometimes.

    const std::map<UserModel, int> resultsRaw = m_database.queryHighScores();
    std::vector<std::pair<UserModel, int>> results(resultsRaw.begin(), resultsRaw.end());

    // Sort before returning
    std::ranges::sort(results,
        [](const auto &a, const auto &b) {
            return a.second > b.second;
        }
    );

    return results;
}

std::optional<UserStatistics> StatisticsManager::getUserStatistics(const unsigned int id) const
{
    return m_database.getUserStatisticsById(id);
}
