#include "GameData.h"

#include "Question.h"

#include <cstdlib>
#include <cmath>

GameData::GameData() : currentQuestionIndex(0),
    correctAnswerCount(0),
    averageAnswerTime(0),
    answersRotation(0)
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
    //TODO: Move this method to a utils method.
    // This is done twice throughout this document.
    this->timeSinceQuestionRoll = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    );

    this->roundTime = std::chrono::milliseconds::zero();
}

void GameData::nextQuestion()
{
    calculateRoundTime();

    this->currentQuestionIndex++;

    calculateRoundPoints();
    rotateAnswers();

    updateTimeSinceQuestionRoll();
}

void GameData::calculateRoundPoints()
{
    int maxTime = this->room.getData().timePerQuestionSecs;
    double time = static_cast<double>(this->roundTime.count());

    double result = 
        (4 * questionPoints / pow(maxTime, 3)) * pow(time, 3) -
        (6 * questionPoints / pow(maxTime, 2)) * pow(time, 2) +
        (3 * questionPoints / maxTime) * time;

    this->points += static_cast<int>(ceil(result));
}

void GameData::calculateRoundTime()
{
    const std::chrono::milliseconds timeNow = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    );

    this->roundTime = timeNow - this->timeSinceQuestionRoll;

    this->averageAnswerTime = std::chrono::duration_cast<std::chrono::seconds>(
        (this->averageAnswerTime * currentQuestionIndex + roundTime)
        / (currentQuestionIndex + 1)
    );
}