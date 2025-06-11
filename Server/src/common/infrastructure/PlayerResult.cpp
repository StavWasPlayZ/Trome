#include "PlayerResult.h"

PlayerResult::PlayerResult(
    const LoggedUser &user,
    const int playtimeSecs,
    const int correctAnswerCount,
    const int averageAnswerTimeSecs,
    const int points
) :
    user(user),
    playtimeSecs(playtimeSecs),
    correctAnswerCount(correctAnswerCount),
    averageAnswerTimeSecs(averageAnswerTimeSecs),
    points(points)
{}

PlayerResult::PlayerResult(const LoggedUser& user, const GameData &data) :
    PlayerResult(
        user,
        data.getPlaytime().count(),
        data.correctAnswerCount,
        data.getAverageAnswerTime().count(),
        data.points
    )
{}
