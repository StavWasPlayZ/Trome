#include "Game.h"

#include "PlayerResult.h"
#include "Utils.h"
#include "manager/RoomManager.h"

#include <stdexcept>

Game::Game(Room &room, const IDatabase &database, RoomManager& roomManager) :
    m_database(database),
    m_roomManager(roomManager),
    m_startTime(0),
    m_room(room),
    m_playersRemaining(0)
{}

Game::~Game()
{
    endGame();
}

unsigned int Game::getId() const
{
    return this->m_room.getId();
}

void Game::startGame()
{
    if (m_room.getStatus() == RoomStatus::PLAYING)
        throw std::runtime_error("Game is already in progress");

    populateQuestions();
    initPlayersData();

    m_playersRemaining = m_room.getAllUsers().size();
    m_startTime = utils::getCurrTimeMillis() + std::chrono::seconds(5);

    m_roomManager.setRoomPlaying(m_room);
}

void Game::endGame() const
{
    m_roomManager.setRoomWaiting(m_room);
}

Room &Game::getRoom() const
{
    return this->m_room;
}

bool Game::isGameComplete() const
{
    return this->m_playersRemaining == 0;
}

int Game::getPlayersRemaining() const
{
    return this->m_playersRemaining;
}

int Game::getPlayersFinished() const
{
    return this->m_room.getAllUsers().size() - this->m_playersRemaining;
}

std::chrono::milliseconds Game::getStartTime() const
{
    return this->m_startTime;
}

const GameData &Game::getDataOf(const LoggedUser &user) const
{
    return this->m_playersData.at(&user);
}

std::vector<PlayerResult> Game::getResults() const
{
    std::vector<PlayerResult> results;

    for (const auto [user, data] : this->m_playersData)
    {
        results.push_back(PlayerResult(*user, data));
    }

    return results;
}

std::optional<UserQuestion> Game::getQuestionForUser(const LoggedUser &user) const
{
    const GameData &data = getDataOf(user);

    if (data.isFinished)
        return std::nullopt;

    return UserQuestion(
        this->m_questions.at(data.currentQuestionIndex),
        data.getAnswersRotation()
    );
}

std::optional<UserQuestion> Game::generateNewQuestionForUser(const LoggedUser &user, const bool didFail)
{
    GameData &data = this->m_playersData.at(&user);

    if (data.isFinished)
        return std::nullopt;

    data.nextQuestion(didFail);
    data.isFinished = data.currentQuestionIndex >= this->m_questions.size();

    if (data.isFinished)
    {
        handlePlayerFinished(user);
        return std::nullopt;
    }

    return getQuestionForUser(user);
}

UserQuestion Game::setFirstQuestionForUser(const LoggedUser &user)
{
    this->m_playersData.at(&user).setFirstQuestion();

    return getQuestionForUser(user).value();
}

void Game::handlePlayerFinished(const LoggedUser &user)
{
    submitGameStatsToDB(user);

    // removePlayer(user);
    //NOTE: We do NOT actually remove the player in question, but wait until the game truly finishes.
    // This is so that said player may still be shown in the after-game view.

    m_playersRemaining--;
}

void Game::initPlayersData()
{
    for (const LoggedUser* user : this->m_room.getAllUsers())
    {
        this->m_playersData.emplace(user, GameData(*this));
    }
}

void Game::populateQuestions()
{
    std::list<Question> questions;

    if constexpr (!MOCK)
    {
        questions = this->m_database.queryQuestions(m_room.getData().questionsCount);
    }
    else
    {
        for (size_t i = 0; i < m_room.getData().questionsCount; i++)
        {
            questions.push_back(Question(
                "mirror mirror on the wall, whose the prettiest of them all?",
                {
                    "MMMMEEEEEEEEE 👺",
                    "me!",
                    "no me!",
                    "obviously me!!"
                }
            ));
        }
    }

    this->m_questions = std::vector(questions.begin(), questions.end());
}

void Game::submitGameStatsToDB(const LoggedUser &user) const
{
    const GameData &data = this->m_playersData.at(&user);
    const std::string &username = user.getUsername();

    m_database.addToStats(
        username,
        data.getPlaytime().count(),
        data.currentQuestionIndex, // By this point it should be 1-based and not 0-based.
        data.correctAnswerCount,
        data.points
    );
}

void Game::removePlayer(const LoggedUser &player)
{
    m_playersData.erase(&player);
}
