#include "infrastructure/RoomData.h"

unsigned int RoomData::globalId = 0;

RoomData::RoomData(
    const std::string &name,
    const RoomStatus &status,
    const int maxPlayers,
    const int timePerQuestion,
    const int questionsCount
) :
    name(name),
    id(globalId++),
    // REVIEW: Perhaps could be initialized directly.
    status(status),
    maxPlayers(maxPlayers),
    timePerQuestion(timePerQuestion),
    questionsCount(questionsCount)
{}
