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

    m_startTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    );
    m_room.setStatus(RoomStatus::PLAYING);
}

void Game::endGame() const
{
    m_room.setStatus(RoomStatus::WAITING);
    // TODO: Self-remove from GameManager
}

Room &Game::getRoom() const
{
    return this->m_room;
}

UserQuestion Game::getQuestionForUser(const LoggedUser &user) const
{
    const GameData &data = this->m_playersData.at(&user);

    return UserQuestion(
        this->m_questions.at(data.currentQuestionIndex),
        data.answersRotation
    );
}

bool Game::generateNewQuestionForUser(const LoggedUser &user)
{
    GameData &data = this->m_playersData.at(&user);

    data.nextQuestion();
    const bool finished = data.currentQuestionIndex < this->m_questions.size();

    if (finished)
    {
        submitGameStatsToDB(user);
        // removePlayer(user);
    }

    return finished;
}

void Game::handleUserLeft(const LoggedUser &user)
{
    removePlayer(user);

    // Think you can get away?
    // ehe~
    submitGameStatsToDB(user);

    // ⠀⠀⠀⠀⢀⠎⠂⠀⠀⠀⣀⣠⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧
    // ⠀⡰⠓⠈⠡⠀⢀⣠⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    // ⠀⡇⠀⠀⣀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⣿⣿⣿⣿
    // ⢀⣰⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣻⣿⠃⢸⣿⣿⣿⣿⣿⣿
    // ⠀⠙⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠋⢠⣿⠏⠀⠀⢿⣿⣿⣿⣿⣿
    // ⠀⠀⠈⣹⣿⣿⣿⣿⡿⠿⣻⣿⣿⣟⡉⠁⠀⠀⢠⣿⠟⠈⠉⠒⠨⢿⣿⣿⣿⣿
    // ⠀⣠⣾⡿⠟⣿⣿⣿⡇⠈⠏⠭⠜⠚⢻⡆⠀⣠⠿⠁.⠳     ⢿⣿⣿
    // ⠀⠀⠀⠀⠀⢸⣿⣿⣅⠀⠐⠄⣈⡒⠚⠁⠞⠁⠀⠀ ...-. ⣿⣿⣿
    // ⠀⠀⠀⠀⠀⣴⣿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⠋⢀⣾⣿⣿⣿
    // ⠀⠀⠀⠀⠀⣿⣿⢿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⡏⠀
    // ⠀⠀⠀⠀⡜⠿⢡⠋⣿⢻⣿⣆⡀⠀⠈⠳⢂⣤⡤⠄⠀⠀⠀⣠⣿⣽⣿⣿⡇⠀
    // ⠀⠀⠀⠀⠇⠆⡇⠀⠀⣸⠥⠻⣯⠂⢄⠀⠀ ⠀⣀⣤⢴⣿⢹⠇⠁⣿⡏⠀⠀
    // ⠀⠀⠀⠀⠃⡰⠁⡠⠊⠀⠀⠀⠇⠑⡤⣉⣒⡂⠅⠊⡇⠀⠙⠐⡴⢂⢀⠇⠀⠀
    // ⠀⠀⠀⠀⠎⠀⠎⠀⠀⠀⢀⠔⠃⡞⢀⠁⠇⡇⢱⠀⢇⡀⠀⢰⠀⢾⠃⠀⠀⠀
    // ⠀⠀⠈⠀⠄⡚⠀⢀⣠⠤⢈⣢⡔⠀⡘⠀⠀⢁⠀⢢⣘⣁⣀⣸⡴⠜⢧⢀⡀⠀
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
    const std::list<Question> questions = this->m_database.queryQuestions(m_room.getData().questionsCount);
    this->m_questions = std::vector(questions.begin(), questions.end());
}

void Game::submitGameStatsToDB(const LoggedUser &user) const
{
    const GameData &data = this->m_playersData.at(&user);
    const std::string &username = user.getUsername();

    const std::chrono::milliseconds currTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    );
    const std::chrono::seconds gameplayTime = std::chrono::duration_cast<std::chrono::seconds>(
        currTime - this->m_startTime
    );

    m_database.addTime(username, gameplayTime.count());

    m_database.addCorrectAns(username, data.correctAnswerCount);
    m_database.addPoints(username, data.points);
    m_database.addTotalAns(username, data.currentQuestionIndex); // By this point it should be 1-based and not 0-based.

    m_database.addGamesPlayed(username);
}

void Game::removePlayer(const LoggedUser &player)
{
    m_playersData.erase(&player);
}

UserQuestion::UserQuestion(const Question &question, const int rotation) :
    question(question),
    rotation(rotation)
{}
