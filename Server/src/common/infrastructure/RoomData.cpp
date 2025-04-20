#include "infrastructure/RoomData.h"


RoomData::RoomData(const std::string &name, const unsigned int id, const RoomStatus &status,
                   const unsigned int max_players, const unsigned int time_per_question)
    : name(name), id(id),
      // REVIEW: Perhaps could be initialized directly.
      status(status), maxPlayers(max_players), timePerQuestion(time_per_question)
{
}