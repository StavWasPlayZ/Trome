#pragma once

#include <string>
#include "infrastructure/GameData.h"

struct PlayerResult
{
    PlayerResult(
        const std::string& username,
        int correctAnswerCount,
        int averageAnswerTime,
        int points
    );

    PlayerResult(const LoggedUser& user, const GameData &data);

    //TODO: Change to User type
    const std::string username;
    const int correctAnswerCount;
    const int averageAnswerTime;
    const int points;
};
