#pragma once
#include <string>

struct PlayerResult
{
    PlayerResult(
        const std::string username,
        const unsigned int correctAnswerCount,
        const unsigned int averageAnswerTime,
        const unsigned int points
    );

    const std::string username;
    const unsigned int correctAnswerCount;
    const unsigned int averageAnswerTime;
    const unsigned int points;
};