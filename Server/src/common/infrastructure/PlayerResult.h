#pragma once

#include <string>
#include "infrastructure/GameData.h"

struct PlayerResult
{
    PlayerResult(
        const LoggedUser &user,
        int correctAnswerCount,
        int averageAnswerTimeSecs,
        int points
    );

    PlayerResult(const LoggedUser &user, const GameData &data);

    const LoggedUser& user;
    const int correctAnswerCount;
    const int averageAnswerTimeSecs;
    const int points;
};
