#include "Game.h"

#include <stdexcept>

Game::Game(Room &room, const IDatabase &database) : m_database(database), m_room(room), m_questionsRotation(0)
{}

Game::~Game()
{
    endGame();
}

void Game::startGame()
{
    const RoomData& roomData = m_room.getData();

    if (m_room.getStatus() == RoomStatus::PLAYING)
        throw std::runtime_error("Game is already in progress");

    const std::list<Question> questions = this->m_database.queryQuestions(roomData.questionsCount);
    this->m_questions = std::vector(questions.begin(), questions.end());

    this->m_questionsRotation = std::rand() % 4;

    m_room.setStatus(RoomStatus::PLAYING);
}

void Game::endGame() const
{
    m_room.setStatus(RoomStatus::WAITING);
}
