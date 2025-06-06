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
    std::sort(
        results.begin(), results.end(),
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
