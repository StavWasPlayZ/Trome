#pragma once

#include <unordered_map>

#include "infrastructure/db/IDatabase.h"
#include "infrastructure/Game.h"
#include "infrastructure/Room.h"

class GameManager
{
public:
    explicit GameManager(const IDatabase& database, RoomManager& roomManager);

  /*
     * PLEASE CALL VIA Room::createNewGame.
     */
    Game &createGame(Room &room);

    static Game &getGameByRoom(const Room &room);

	void deleteGame(const Game &game);

private:
	const IDatabase& m_database;
    RoomManager& m_roomManager;

	std::unordered_map<unsigned int, Game> m_games;
};
