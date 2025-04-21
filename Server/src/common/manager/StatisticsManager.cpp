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

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string &username) const
{
    //TODO: Convert to KVP or object of results.

    std::vector<std::string> result;

    result.push_back(
        "Games played: " + std::to_string(m_database.queryGamesPlayed(username))
    );
    result.push_back(
        "Questions answered: " + std::to_string(m_database.queryTotalAns(username))
    );
    result.push_back(
        "Questions answered correctly: " + std::to_string(m_database.queryCorrectAns(username))
    );
    result.push_back(
        "Time took to answer all of the questions: " + std::to_string(m_database.queryTime(username))
    );
    result.push_back(
        "Average time per question: " + std::to_string(m_database.queryPlayerAverageAnsTime(username))
    );
    result.push_back(
        "Total points earned: " + std::to_string(m_database.queryPoints(username))
    );

    return result;
}
