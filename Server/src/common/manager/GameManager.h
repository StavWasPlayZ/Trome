#pragma once

#include <unordered_map>

#include "infrastructure/db/IDatabase.h"
#include "infrastructure/Game.h"
#include "infrastructure/Room.h"
#include "infrastructure/Question.h"
#include "manager/LoggedUser.h"
#include "handler/codec/s2c/response/Response.h" // for ProtocolRequest
#include "handler/codec/s2c/response/ErrorResponse.h" // for ErrorResponse
#include "handler/codec/c2s/request/Request.h" // for RequestInfo

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
    ProtocolResponse *addQuestion(const RequestInfo &info, const Question &question, const LoggedUser &user);

private:
	const IDatabase& m_database;
    RoomManager& m_roomManager;

	std::unordered_map<unsigned int, Game> m_games;
};
