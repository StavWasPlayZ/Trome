#pragma once

#include "infrastructure/Room.h"
#include <chrono>

struct UserQuestion;
struct Question;

/**
 * Represents the data of a player within a running game
 */
struct GameData
{
    explicit GameData(const Game& game);

    void rotateAnswers();
    void updateTimeSinceQuestionRoll();

    void nextQuestion(bool didFail);


    std::chrono::milliseconds getTimeSinceQuestionRoll() const;
    std::chrono::seconds getAverageAnswerTime() const;
    int getAnswersRotation() const;

    std::chrono::milliseconds getRoundTime() const;


    size_t currentQuestionIndex;
    bool isFinished;

    int correctAnswerCount;
    int points;

private:
    static constexpr int QUESTION_POINTS = 500;
    static constexpr int FAILURE_PENALTY = QUESTION_POINTS / 4;

    // Graph: https://www.desmos.com/calculator/a4sauosvwr
    void calculateRoundPoints(bool didFail);

    /**
     * Updates how long the current round has taken into GameData::roundTime.
     *
     * Also updates GameData::averageAnswerTime and GameData::timeSinceQuestionRoll accordingly.
     */
    void submitRoundTime();


    std::chrono::seconds averageAnswerTime;

    /**
     * The amount of which to offset the answers' IDs provided to the client.
     */
    int answersRotation;

    std::chrono::milliseconds roundTime;
    std::chrono::milliseconds timeSinceQuestionRoll;

    const Game &game;
};


struct UserQuestion
{
    UserQuestion(const Question& question, int rotation);

    const Question& question;
    const int rotation;
};
