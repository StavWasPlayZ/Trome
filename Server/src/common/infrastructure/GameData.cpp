#include "GameData.h"

#include "Question.h"

#include <cstdlib>

GameData::GameData() : currentQuestionIndex(0),
    correctAnswerCount(0),
    wrongAnswerCount(0),
    averageAnswerTime(0),
    answersRotation(0)
{
    rotateAnswers();
}

void GameData::rotateAnswers()
{
    this->answersRotation = std::rand() % Question::QUESTIONS_AMOUNT;
}