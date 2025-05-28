#pragma once

#include <chrono>
#include "infrastructure/Room.h"

struct Question;

/**
 * Represents the data of a player within a running game
 */
struct GameData
{
    explicit GameData(const Game& game);

    void rotateAnswers();
    void updateTimeSinceQuestionRoll();

    void nextQuestion();


    size_t currentQuestionIndex;
    std::chrono::milliseconds timeSinceQuestionRoll;

    int correctAnswerCount;
    int points;

    std::chrono::seconds averageAnswerTime;

    /**
     * The amount of which to offset the answers' IDs provided to the client.
     */
    int answersRotation;

private:
    static constexpr int QUESTION_POINTS = 500;

    void calculateRoundPoints();

    /**
     * Sets and updates both GameData::roundTime and GameData::averageAnswerTime.
     */
    void calculateRoundTime();

    std::chrono::milliseconds roundTime;

    const Game &game;
};
