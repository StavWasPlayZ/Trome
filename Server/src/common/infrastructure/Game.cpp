#include "Game.h"

Game::Game(Room &room, const IDatabase &database) : m_database(database), m_room(room), m_questionsRotation(0)
{}

Game::~Game()
{
    endGame();
}

void Game::startGame()
{
    RoomData &roomData = m_room.getData();

    if (roomData.status == RoomStatus::PLAYING)
        throw std::runtime_error("Game is already in progress");

    roomData.status = RoomStatus::PLAYING;

    const std::list<Question> questions = this->m_database.queryQuestions(roomData.questionsCount);
    this->m_questions = std::vector(questions.begin(), questions.end());

    this->m_questionsRotation = std::rand() % 4;
}

void Game::endGame() const
{
    this->m_room.getData().status = RoomStatus::WAITING;
}
