#pragma once

#include <string>
#include "infrastructure/GameData.h"
#include "manager/LoggedUser.h"

struct GameData;
struct LoggedUser;

struct PlayerResult
{
    PlayerResult(
        const std::string& username,
        int correctAnswerCount,
        int averageAnswerTime,
        int points
    );

    // note: user isn't const because getUsername doesn't work with const LoggedUser
    PlayerResult(LoggedUser& user, const GameData &data);

    //TODO: Change to User type
    const std::string username;
    const int correctAnswerCount;
    const int averageAnswerTime;
    const int points;
};