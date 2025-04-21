#include "infrastructure/RoomData.h"

unsigned int RoomData::globalId = 0;

RoomData::RoomData(
    const std::string &name,
    const RoomStatus &status,
    const unsigned int max_players, const unsigned int time_per_question
) :
    name(name),
    id(globalId++),
    // REVIEW: Perhaps could be initialized directly.
    status(status),
    maxPlayers(max_players),
    timePerQuestion(time_per_question)
{}
