#pragma once

/**
 * DO NOT INCLUDE "infrastructure/RoomData.h".
 *
 * this file already includes manager/LoggedUser.h
 * ...that includes infrastructure/Client.h
 * ...that includes handler/IRequestHandler.h
 * ...that includes infrastructure/request/RequestInfo.h
 * ...which has enum class RequestCode : unsigned char;
 * ...which includes "infrastructure/RoomData.h".
 */

#include "db/IDatabase.h"

#include <vector>
#include <stack>

#include "manager/LoggedUser.h"
#include "infrastructure/Question.h"

class Room
{
public:
    /**
     * Constructs a new Room.
     *
     * Users should be added manually via Room::addUser.
     */
    Room(LoggedUser& admin, const RoomData& data, const IDatabase& database);

    /**
     * Starts the game for this room, populating it with questions
     * and randomizing the rotation.
     */
    void startGame();
    void endGame();

    const Question& getCurrentQuestion() const;
    /**
     * Moves on to the next question.
     *
     * Returns: Whether more questions are available
    */
    bool nextQuestion();

    void addUser(LoggedUser& user);
    void removeUser(const LoggedUser& user);
    std::vector<LoggedUser*> getAllUsers() const;

    RoomData& getData();

    LoggedUser& getAdmin() const;
    void setAdmin(LoggedUser& admin);

private:
    // Made a pointer such that if we'd like to change it in the future
    LoggedUser* m_admin;

    RoomData m_metadata;
    std::vector<LoggedUser*> m_users;

    std::stack<Question> m_questions;
    int m_questionsRotation;

    const IDatabase& m_database;
};