#include "Game.h"

#include "Utils.h"

#include <stdexcept>

Game::Game(Room &room, const IDatabase &database) :
    m_database(database),
    m_startTime(0),
    m_room(room),
    playersRemaining(0)
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

    m_startTime = utils::getCurrTimeMillis();
    m_room.setStatus(RoomStatus::PLAYING);
}

void Game::endGame() const
{
    m_room.setStatus(RoomStatus::WAITING);
}

Room &Game::getRoom() const
{
    return this->m_room;
}

bool Game::isGameComplete() const
{
    return this->playersRemaining == 0;
}

const GameData &Game::getDataOf(const LoggedUser &user) const
{
    return this->m_playersData.at(&user);
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
        handleUserLeft(user);
        return std::nullopt;
    }

    return getQuestionForUser(user);
}

UserQuestion Game::setFirstQuestionForUser(const LoggedUser &user)
{
    this->m_playersData.at(&user).setFirstQuestion();

    return getQuestionForUser(user).value();
}

void Game::handleUserLeft(const LoggedUser &user)
{
    // removePlayer(user);

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
    // ⠀⠀⠀⠀⠀⢸⣿⣿⣅⠀⠐⠄⣈⡒⠚⠁⠞⠁⠀⠀ .---. ⣿⣿⣿
    // ⠀⠀⠀⠀⠀⣴⣿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀  ⢀⣾⣿⣿⣿
    // ⠀⠀⠀⠀⠀⣿⣿⢿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⡏⠀
    // ⠀⠀⠀⠀⡜⠿⢡⠋⣿⢻⣿⣆⡀⠀⠈⠳⢂⣤⡤⠄⠀⠀⠀⣠⣿⣽⣿⣿⡇⠀
    // ⠀⠀⠀⠀⠇⠆⡇⠀⠀⣸⠥⠻⣯⠂⢄⠀⠀ ⠀⣀⣤⢴⣿⢹⠇⠁⣿⡏⠀⠀
    // ⠀⠀⠀⠀⠃⡰⠁⡠⠊⠀⠀⠀⠇⠑⡤⣉⣒⡂⠅⠊⡇⠀⠙⠐⡴⢂⢀⠇⠀⠀
    // ⠀⠀⠀⠀⠎⠀⠎⠀⠀⠀⢀⠔⠃⡞⢀⠁⠇⡇⢱⠀⢇⡀⠀⢰⠀⢾⠃⠀⠀⠀
    // ⠀⠀⠈⠀⠄⡚⠀⢀⣠⠤⢈⣢⡔⠀⡘⠀⠀⢁⠀⢢⣘⣁⣀⣸⡴⠜⢧⢀⡀⠀


    //NOTE: We do not actually remove the player in question, but wait until the game truly finishes.
    // This is so that said player may still be shown in the after-game view.

    playersRemaining--;
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

    const std::chrono::seconds gameplayTime = std::chrono::duration_cast<std::chrono::seconds>(
        utils::getCurrTimeMillis() - this->m_startTime
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
