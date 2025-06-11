#include "GameData.h"

#include "Game.h"
#include "Question.h"
#include "Utils.h"

#include <cmath>

GameData::GameData(const Game& game) :
    currentQuestionIndex(-1),
    isFinished(false),
    correctAnswerCount(0),
    points(0),
    averageAnswerTime(0),
    answersRotation(0),
    game(game)
{
    rotateAnswers();
    updateTimeSinceQuestionRoll();
}

void GameData::rotateAnswers()
{
    this->answersRotation = std::rand() % Question::QUESTIONS_AMOUNT;
}

void GameData::updateTimeSinceQuestionRoll()
{
    this->timeSinceQuestionRoll = utils::getCurrTimeMillis();
    this->roundTime = std::chrono::milliseconds::zero();
}

void GameData::setFirstQuestion()
{
    this->currentQuestionIndex = 0;
}

void GameData::nextQuestion(const bool didFail)
{
    if (!didFail)
    {
        this->correctAnswerCount++;
    }

    submitRoundTime();

    this->currentQuestionIndex++;

    calculateRoundPoints(didFail);

    rotateAnswers();
    updateTimeSinceQuestionRoll();
}

bool GameData::didYetStart() const
{
    return this->currentQuestionIndex == -1;
}

std::chrono::milliseconds GameData::getTimeSinceQuestionRoll() const
{
    return this->timeSinceQuestionRoll;
}

std::chrono::seconds GameData::getAverageAnswerTime() const
{
    // if the time is 0 (rounded) return 1 sec
    return this->averageAnswerTime.count() > 0 ? this->averageAnswerTime : std::chrono::seconds(1);
}

std::chrono::seconds GameData::getPlaytime() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(
        this->timeSinceQuestionRoll - this->game.getStartTime()
    );
}

int GameData::getAnswersRotation() const
{
    return this->answersRotation;
}

std::chrono::milliseconds GameData::getRoundTime() const
{
    return utils::getCurrTimeMillis() - this->timeSinceQuestionRoll;
}

void GameData::calculateRoundPoints(const bool didFail)
{
    if (didFail)
    {
        this->points -= FAILURE_PENALTY;
        return;
    }

    const double maxTime = this->game.getRoom().getData().timePerQuestionSecs * 1000;
    const double timeLeft = maxTime - static_cast<double>(this->roundTime.count());

    const double result =
        (4 * QUESTION_POINTS / pow(maxTime, 3)) * pow(timeLeft, 3) -
        (6 * QUESTION_POINTS / pow(maxTime, 2)) * pow(timeLeft, 2) +
        (3 * QUESTION_POINTS / maxTime) * timeLeft;

    this->points += static_cast<int>(ceil(result));
}

void GameData::submitRoundTime()
{
    this->roundTime = getRoundTime();

    this->averageAnswerTime = std::chrono::duration_cast<std::chrono::seconds>(
        (this->averageAnswerTime * currentQuestionIndex + roundTime)
        / (currentQuestionIndex + 1)
    );
}

UserQuestion::UserQuestion(const Question &question, const int rotation) :
    question(question),
    rotation(rotation)
{}
