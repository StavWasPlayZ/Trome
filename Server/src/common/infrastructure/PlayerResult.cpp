#include "PlayerResult.h"

PlayerResult::PlayerResult(
    const std::string &username,
    const int correctAnswerCount,
    const int averageAnswerTime,
    const int points
) : 
    username(username),
    correctAnswerCount(correctAnswerCount),
    averageAnswerTime(averageAnswerTime),
    points(points)
{}
