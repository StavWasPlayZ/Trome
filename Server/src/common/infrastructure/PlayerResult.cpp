#include "PlayerResult.h"

PlayerResult::PlayerResult(
    const std::string username,
    const unsigned int correctAnswerCount,
    const unsigned int averageAnswerTime,
    const unsigned int points
) : 
    username(username),
    correctAnswerCount(correctAnswerCount),
    averageAnswerTime(averageAnswerTime),
    points(points)
{}
