#pragma once

#include "GameData.h"

#include <unordered_map>
#include <chrono>

#include "Room.h"
#include "db/IDatabase.h"

#include "infrastructure/Question.h"

struct UserQuestion;

class Game
{
public:
    Game(Room &room, const IDatabase &database);
    ~Game();

    unsigned int getId() const;

    /**
     * Starts the game for this room, populating it with questions.
     */
    void startGame();
    void endGame() const;

    const Room& getRoom() const;

    UserQuestion getQuestionForUser(const LoggedUser& user) const;
    /**
     * Returns true whether a new question was generated, false otherwise.
     *
     * A question may not be generated if the user has finished answering all set questions.
     */
    bool generateNewQuestionForUser(const LoggedUser& user);

private:
    void initPlayersData();
    void populateQuestions();

    void submitGameStatsToDB(const LoggedUser &user) const;

    /**
     * Removes the provided player's data from the game.
     * Used for when the player finished the game (early) (probably).
     */
    void removePlayer(const LoggedUser& player);

    const IDatabase &m_database;

    std::chrono::milliseconds m_startTime;

    // Here, you can already find the room ID (sufficient itself as the game ID)
    Room &m_room;
    std::unordered_map<const LoggedUser*, GameData> m_playersData;

    std::vector<Question> m_questions;
};


struct UserQuestion
{
    UserQuestion(const Question& question, int rotation);

    const Question& question;
    int rotation;
};
