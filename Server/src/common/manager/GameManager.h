#pragma once

#include <unordered_map>

#include "infrastructure/db/IDatabase.h"
#include "infrastructure/Game.h"
#include "infrastructure/Room.h"

class GameManager
{
public:
    explicit GameManager(const IDatabase& m_database);

    /*
     * Creates a new game, and automatically associates it to the provided room
     */
    Game &createGame(Room &room);

    static Game &getGameByRoom(const Room &room);

	void deleteGame(Room &room);

private:
	const IDatabase& m_database;
	std::unordered_map<unsigned int, Game> m_games;
};
