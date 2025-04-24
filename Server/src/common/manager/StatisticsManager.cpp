#include "StatisticsManager.h"

#include <algorithm>

StatisticsManager::StatisticsManager(const IDatabase& db) :
    m_database(db)
{}

std::vector<std::pair<std::string, int>> StatisticsManager::getHighScores() const
{
    //TODO: Cache this. Only update once per sometimes.

    const std::unordered_map<std::string, int> resultsRaw = m_database.queryHighScores();

    // Sort before returning
    std::vector<std::pair<std::string, int>> results(resultsRaw.begin(), resultsRaw.end());

    std::ranges::sort(results,
        [](const auto &a, const auto &b) {
            return a.second > b.second;
        }
    );

    return results;
}

UserStatistics StatisticsManager::getUserStatistics(const std::string &username) const
{
    return UserStatistics(
        m_database.queryGamesPlayed(username),
        m_database.queryTotalAns(username),
        m_database.queryCorrectAns(username),
        m_database.queryTime(username),
        m_database.queryPlayerAverageAnsTime(username),
        m_database.queryPoints(username)
    );
}
