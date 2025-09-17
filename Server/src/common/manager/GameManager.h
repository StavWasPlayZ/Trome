#pragma once

#include <unordered_map>

#include "infrastructure/db/IDatabase.h"
#include "infrastructure/Game.h"
#include "infrastructure/Question.h"
#include "manager/LoggedUser.h"

class GameManager
{
public:
    GameManager(const IDatabase& database, RoomManager& roomManager);

    /*
     * PLEASE CALL VIA Room::createNewGame.
     */
    Game &createGame(Room &room);

    static Game &getGameByRoom(const Room &room);

	void deleteGame(const Game &game);

    /**
     * Adds a question to the DB
     *
     * NOTE: THE RETURNED RESOURCE MUST BE FREED
     */
    void addQuestion(const Question &question, const LoggedUser &user) const;

private:
	const IDatabase& m_database;
    RoomManager& m_roomManager;

	std::unordered_map<long, Game> m_games;
};
