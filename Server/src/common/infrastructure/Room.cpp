#include "Room.h"

#include <ctime>

#include <list>

#include "db/IDatabase.h"

Room::Room(LoggedUser &admin, const RoomData &data, const IDatabase& database) :
    m_admin(&admin),
    m_metadata(data),
    m_questionsRotation(0),
    m_database(database)
{
    // Add the admin to the room
    addUser(admin);
}

void Room::startGame()
{
    this->m_metadata.status = RoomStatus::PLAYING;

    const std::list<Question> questions = this->m_database.queryQuestions(this->m_metadata.questionsCount);

    for (const Question &question : questions)
    {
        this->m_questions.push(question);
    }

    this->m_questionsRotation = std::rand() % 4;
}

void Room::endGame()
{
    this->m_metadata.status = RoomStatus::WAITING;

    // Clear out all questions
    while (!this->m_questions.empty())
    {
        this->m_questions.pop();
    }

    this->m_questionsRotation = 0;
}

const Question &Room::getCurrentQuestion() const
{
    if (this->m_questions.size() == 0)
    {
        throw std::runtime_error("Room is not populated with any questions; Game has either ended or yet to begin.");
    }

    return this->m_questions.top();
}

bool Room::nextQuestion()
{
    this->m_questions.pop();
    return !this->m_questions.empty();
}

void Room::addUser(LoggedUser &user)
{
    this->m_users.push_back(&user);
}

void Room::removeUser(const LoggedUser &user)
{
    const auto it = std::ranges::find(this->m_users, &user);

    if (it != m_users.end())
    {
        m_users.erase(it);
    }
}

std::vector<LoggedUser*> Room::getAllUsers() const
{
    return this->m_users;
}

RoomData &Room::getData()
{
    return this->m_metadata;
}

LoggedUser &Room::getAdmin() const
{
    return *m_admin;
}

void Room::setAdmin(LoggedUser &admin)
{
    this->m_admin = &admin;
}
