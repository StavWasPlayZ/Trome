#pragma once

#include "Room.h"
#include "db/IDatabase.h"

#include "infrastructure/Question.h"

class Game
{
    //TODO: Implement the rest in v4.0.0

public:
    Game(Room &room, const IDatabase &database);
    ~Game();

    /**
     * Starts the game for this room, populating it with questions and randomizing the rotation.
     */
    void startGame();
    void endGame() const;

private:
    const IDatabase &m_database;

    // Here, you can already find the room ID (sufficient itself as the game ID)
    // - List of all players
    Room &m_room;

    std::vector<Question> m_questions;
    int m_questionsRotation;
};
