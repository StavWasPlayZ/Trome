#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(const IDatabase& db) :
    m_database(db)
{}

std::vector<std::pair<std::string, int>> StatisticsManager::getHighScore() const
{
    return m_database.queryHighScores();
}

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string &username) const
{
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
