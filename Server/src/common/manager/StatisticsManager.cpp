#include "StatisticsManager.h"

#include <sstream>
#include <iostream>

StatisticsManager::StatisticsManager(IDatabase *db) : m_database(db)
{
}

std::vector<std::pair<std::string, int>> StatisticsManager::getHighScore()
{
    return m_database->getHighScores();
}

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string &username)
{
    std::vector<std::string> v;
    std::ostringstream oss;

    oss << "Games played: " << m_database->getGamesPlayed(username);
    v.push_back(oss.str());
    oss.str("");
    oss.clear();

    oss << "Questions answered: " << m_database->getTotalAns(username);
    v.push_back(oss.str());
    oss.str("");
    oss.clear();

    oss << "Questions answered correctly: " << m_database->getCorrectAns(username);
    v.push_back(oss.str());
    oss.str("");
    oss.clear();

    oss << "Time took to answer all of the questions: " << m_database->getTime(username);
    v.push_back(oss.str());
    oss.str("");
    oss.clear();

    oss << "Average time per question: " << m_database->getPlayerAverageAnsTime(username);
    v.push_back(oss.str());
    oss.str("");
    oss.clear();

    oss << "Total points earned: " << m_database->getPoints(username);
    v.push_back(oss.str());
    oss.str("");
    oss.clear();

    return v;
}
