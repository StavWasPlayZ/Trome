#include "GameManager.h"

GameManager::GameManager(const IDatabase& m_database) : m_database(m_database)
{}

Game& GameManager::createGame(Room &room)
{
    const auto [result, _] = this->m_games.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(room.getId()),
        std::forward_as_tuple(room, m_database)
    );

    return result->second;
}

Game &GameManager::getGameByRoom(const Room &room)
{
    const std::optional<Game*> game = room.getCurrentGame();

    if (!game.has_value())
        throw std::runtime_error("Game not found");

    return *game.value();
}

void GameManager::deleteGame(Room &room)
{
    room.unsetCurrentGame();
    this->m_games.erase(room.getId());
}
