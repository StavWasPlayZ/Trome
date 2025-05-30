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

    //TODO: Change to User type
    const std::string username;
    const int correctAnswerCount;
    const int averageAnswerTime;
    const int points;
};