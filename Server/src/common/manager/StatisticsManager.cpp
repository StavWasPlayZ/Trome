#include "StatisticsManager.h"

StatisticsManager::StatisticsManager(IDatabase *db) : m_database(db)
{
}

std::vector<std::pair<std::string, int>> StatisticsManager::getHighScore()
{
    return m_database->getHighScores();
}