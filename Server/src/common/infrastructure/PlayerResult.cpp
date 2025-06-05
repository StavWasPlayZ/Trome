#include "PlayerResult.h"

PlayerResult::PlayerResult(
    const LoggedUser &user,
    const int correctAnswerCount,
    const int averageAnswerTimeSecs,
    const int points
) :
    user(user),
    correctAnswerCount(correctAnswerCount),
    averageAnswerTimeSecs(averageAnswerTimeSecs),
    points(points)
{}

PlayerResult::PlayerResult(const LoggedUser& user, const GameData &data) :
    user(user),
    correctAnswerCount(data.correctAnswerCount),
    averageAnswerTimeSecs(data.getAverageAnswerTime().count()),
    points(data.points)
{}
