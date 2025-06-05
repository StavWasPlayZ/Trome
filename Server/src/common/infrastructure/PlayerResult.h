#pragma once

#include <string>
#include "infrastructure/GameData.h"

struct PlayerResult
{
    PlayerResult(
        const LoggedUser &user,
        int playtimeSecs,
        int correctAnswerCount,
        int averageAnswerTimeSecs,
        int points
    );

    PlayerResult(const LoggedUser &user, const GameData &data);

    const LoggedUser& user;
    const int playtimeSecs;
    const int correctAnswerCount;
    const int averageAnswerTimeSecs;
    const int points;
};
