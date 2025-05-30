#include "GameData.h"

#include "Game.h"
#include "Question.h"
#include "Utils.h"

#include <cmath>

GameData::GameData(const Game& game) :
    currentQuestionIndex(0),
    correctAnswerCount(0),
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

void GameData::nextQuestion(const bool didFail)
{
    if (!didFail)
    {
        this->correctAnswerCount++;
    }

    calculateRoundTime();

    this->currentQuestionIndex++;

    calculateRoundPoints(didFail);
    rotateAnswers();
}

void GameData::calculateRoundPoints(const bool didFail)
{
    if (didFail)
    {
        this->points -= FAILURE_PENALTY;
        return;
    }

    const int maxTime = this->game.getRoom().getData().timePerQuestionSecs;
    const double time = static_cast<double>(this->roundTime.count());

    const double result =
        (4 * QUESTION_POINTS / pow(maxTime, 3)) * pow(time, 3) -
        (6 * QUESTION_POINTS / pow(maxTime, 2)) * pow(time, 2) +
        (3 * QUESTION_POINTS / maxTime) * time;

    this->points += static_cast<int>(ceil(result));
}

void GameData::calculateRoundTime()
{
    this->roundTime = utils::getCurrTimeMillis() - this->timeSinceQuestionRoll;
    updateTimeSinceQuestionRoll();

    this->averageAnswerTime = std::chrono::duration_cast<std::chrono::seconds>(
        (this->averageAnswerTime * currentQuestionIndex + roundTime)
        / (currentQuestionIndex + 1)
    );
}