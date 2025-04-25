#include "Game.h"

Game::Game(Room &room, const IDatabase &database) : m_database(database), m_room(room), m_questionsRotation(0)
{}

Game::~Game()
{
    endGame();
}

void Game::startGame()
{
    const RoomData& roomData = m_room.getData();

    if (roomData.status == RoomStatus::PLAYING)
        throw std::runtime_error("Game is already in progress");

    const std::list<Question> questions = this->m_database.queryQuestions(roomData.questionsCount);
    this->m_questions = std::vector(questions.begin(), questions.end());

    this->m_questionsRotation = std::rand() % 4;


    RoomData newRoomData = RoomData(roomData);
    newRoomData.status = RoomStatus::PLAYING;
    m_room.setData(newRoomData);
}

void Game::endGame() const
{
    RoomData newRoomData = RoomData(m_room.getData());
    newRoomData.status = RoomStatus::WAITING;
    m_room.setData(newRoomData);
}
