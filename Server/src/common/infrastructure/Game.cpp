#include "Game.h"

#include <stdexcept>

Game::Game(Room &room, const IDatabase &database) :
    m_database(database),
    m_room(room)
{}

Game::~Game()
{
    endGame();
}

void Game::startGame()
{
    if (m_room.getStatus() == RoomStatus::PLAYING)
        throw std::runtime_error("Game is already in progress");

    populateQuestions();
    initPlayersData();

    m_room.setStatus(RoomStatus::PLAYING);
}

void Game::endGame() const
{
    m_room.setStatus(RoomStatus::WAITING);
    //TODO: Self-remove from GameManager
}

unsigned int Game::getId() const
{
    return this->m_room.getId();
}

void Game::initPlayersData()
{
    for (const LoggedUser* user : this->m_room.getAllUsers())
    {
        this->m_playersData.emplace(user, GameData());
    }
}

void Game::populateQuestions()
{
    const std::list<Question> questions = this->m_database.queryQuestions(m_room.getData().questionsCount);
    this->m_questions = std::vector(questions.begin(), questions.end());
}
