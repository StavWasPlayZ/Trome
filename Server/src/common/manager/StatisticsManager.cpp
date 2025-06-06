#include "StatisticsManager.h"

#include "infrastructure/Server.h"

#include <algorithm>

StatisticsManager::StatisticsManager(const IDatabase& db) :
    m_database(db)
{}

std::vector<std::pair<LoggedUser*, int>> StatisticsManager::getHighScores() const
{
    //TODO: Cache this. Only update once per sometimes.

    const std::unordered_map<std::string, int> resultsRaw = m_database.queryHighScores();
    std::vector<std::pair<LoggedUser*, int>> results;

    std::ranges::transform(
        resultsRaw,
        std::back_inserter(results),
        [](const std::pair<const std::string, int>& entry) {
            return std::make_pair(
                &Server::getInstance().getLoginManager().getUserByUsername(entry.first),
                entry.second
            );
        }
    );

    // Sort before returning
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
