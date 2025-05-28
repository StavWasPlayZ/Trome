#pragma once

struct Question;

/**
 * Represents the data of a player within a running game
 */
struct GameData
{
    GameData();

    size_t currentQuestionIndex;

    int correctAnswerCount;
    int wrongAnswerCount;
    int averageAnswerTime;

    /**
     * The amount of which to offset the answers' IDs provided to the client.
     */
    int answersRotation;

    void rotateAnswers();
};
