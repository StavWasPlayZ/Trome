#include "PlayerResult.h"

PlayerResult::PlayerResult(
    const LoggedUser &user,
    const int correctAnswerCount,
    const int averageAnswerTime,
    const int points
) :
    user(user),
    correctAnswerCount(correctAnswerCount),
    averageAnswerTime(averageAnswerTime),
    points(points)
{}

PlayerResult::PlayerResult(const LoggedUser& user, const GameData &data) :
    user(user),
    correctAnswerCount(data.correctAnswerCount),
    averageAnswerTime(data.getAverageAnswerTime().count()),
    points(data.points)
{}
