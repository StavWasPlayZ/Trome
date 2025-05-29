#pragma once

#include <vector>

#include "infrastructure/db/IDatabase.h"
#include "infrastructure/Game.h"
#include "infrastructure/Room.h"

class GameManager
{
public:
    explicit GameManager(const IDatabase& m_database);

	Game& createGame(Room &room);

	Game& getGameByRoom(const Room &room);

	void deleteGame(Room &room);

private:
	const IDatabase& m_database;
	std::vector<Game &> m_games;
};