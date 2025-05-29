#include "GameManager.h"

GameManager::GameManager(const IDatabase& m_database) : m_database(m_database)
{}

Game& GameManager::createGame(Room &room)
{
    Game game(room, this->m_database);
    room.setCurrentGame(game);
    this->m_games.emplace_back(game);
    return game;
}

Game &GameManager::getGameByRoom(const Room &room)
{
    for (auto& game : this->m_games)
    {
        if (game.getId() == room.getId())
        {
            return game;
        }
    }

    throw std::runtime_error("Game not found");
}

void GameManager::deleteGame(Room &room)
{
    for (auto it = m_games.begin(); it != m_games.end(); ++it)
    {
        if (it->getId() == room.getId())
        {
            // TODO: make this to set the room's game to be null
            // game.getRoom().setCurrentGame(...);
            m_games.erase(it); // erase returns iterator to the next element
            break;
        }
    }
}
