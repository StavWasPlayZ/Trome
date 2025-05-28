#pragma once

#include "GameData.h"

#include <unordered_map>

#include "Room.h"
#include "db/IDatabase.h"

#include "infrastructure/Question.h"

class Game
{
public:
    Game(Room &room, const IDatabase &database);
    ~Game();

    /**
     * Starts the game for this room, populating it with questions.
     */
    void startGame();
    void endGame() const;

    unsigned int getId() const;

private:
    const IDatabase &m_database;

    void initPlayersData();
    void populateQuestions();

    // Here, you can already find the room ID (sufficient itself as the game ID)
    Room &m_room;
    std::unordered_map<const LoggedUser*, GameData> m_playersData;

    std::vector<Question> m_questions;
};
