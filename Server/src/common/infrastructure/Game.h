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

    Room& getRoom() const;

    /**
     * Returns the active question of the current user, if one exists.
     *
     * A question may not exist if the user has already finished answering them all.
     */
    std::optional<UserQuestion> getQuestionForUser(const LoggedUser& user) const;
    /**
     * Generates a new question for the user and returns the new, active question of the current user, if one exists.
     *
     * A question may not exist if the user has already finished answering them all.
     */
    std::optional<UserQuestion> generateNewQuestionForUser(const LoggedUser& user, bool didFail);

    void handleUserLeft(const LoggedUser& user) const;

private:
    void initPlayersData();
    void populateQuestions();

    void submitGameStatsToDB(const LoggedUser &user) const;

    /**
     * Removes the provided player's data from the game.
     */
    void removePlayer(const LoggedUser& player);

    const IDatabase &m_database;

    std::chrono::milliseconds m_startTime;

    // Here, you can already find the room ID (sufficient itself as the game ID)
    Room &m_room;
    std::unordered_map<const LoggedUser*, GameData> m_playersData;

    std::vector<Question> m_questions;
};
