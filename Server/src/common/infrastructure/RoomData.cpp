#include "infrastructure/RoomData.h"

RoomData::RoomData(const std::string &name, const int maxPlayers, const int timePerQuestion, const int questionsCount) :
    name(name),
    maxPlayers(maxPlayers),
    timePerQuestionSecs(timePerQuestion),
    questionsCount(questionsCount)
{
}

RoomData RoomData::ofDefaults()
{
    return RoomData(
        //TODO: Generate some name from a list
        "NEW ROOM",
        5,
        10,
        25
    );
}

std::chrono::milliseconds RoomData::getTimePerQuestionMs() const
{
    // Adding +1sec for potential server delay directly
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::seconds(timePerQuestionSecs + 1)
    );
}
