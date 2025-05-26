#pragma once
#include <string>

struct PlayerResult
{
    PlayerResult(
        const std::string& username,
        int correctAnswerCount,
        int averageAnswerTime,
        int points
    );

    const std::string username;
    const int correctAnswerCount;
    const int averageAnswerTime;
    const int points;
};