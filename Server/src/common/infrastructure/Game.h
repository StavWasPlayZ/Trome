#pragma once

#include "GameData.h"

#include <unordered_map>
#include <chrono>

#include "Room.h"
#include "db/IDatabase.h"

#include "infrastructure/Question.h"

class RoomManager;
struct PlayerResult;
struct UserQuestion;

class Game
{
public:
    Game(Room &room, const IDatabase &database, RoomManager& roomManager);
    ~Game();

    unsigned int getId() const;

    /**
     * Starts the game for this room, populating it with questions.
     */
    void startGame();
    void endGame() const;

    Room& getRoom() const;

    /**
     * Returns whether all players have answered all questions
     */
    bool isGameComplete() const;

    /**
     * Returns the amount of players who have yet to finish all the questions of the room
     */
    int getPlayersRemaining() const;
    /**
     * Returns the amount of players who have finished all the questions of the room
     */
    int getPlayersFinished() const;


    std::chrono::seconds getPlaytime() const;


    const GameData& getDataOf(const LoggedUser& user) const;

    std::vector<PlayerResult> getResults() const;


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

    UserQuestion setFirstQuestionForUser(const LoggedUser& user);

    void handleUserLeft(const LoggedUser& user);

private:
    /**
     * Defines whether all the questions will be debug nonsense
     */
    static constexpr bool MOCK = false;

    void initPlayersData();
    void populateQuestions();

    void submitGameStatsToDB(const LoggedUser &user) const;

    /**
     * Removes the provided player's data from the game.
     */
    void removePlayer(const LoggedUser& player);

    const IDatabase &m_database;
    RoomManager& m_roomManager;

    std::chrono::milliseconds m_startTime;

    // Here, you can already find the room ID (sufficient itself as the game ID)
    Room &m_room;
    std::unordered_map<const LoggedUser*, GameData> m_playersData;

    std::vector<Question> m_questions;

    int m_playersRemaining;
};
