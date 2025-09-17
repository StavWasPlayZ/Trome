#include "infrastructure/RoomData.h"

#include "Room.h"

RoomData::RoomData(const std::string &name, const int maxPlayers, const int timePerQuestion, const int questionsCount) :
    name(name),
    maxPlayers(maxPlayers),
    timePerQuestionSecs(timePerQuestion),
    questionsCount(questionsCount)
{
}

RoomData RoomData::ofDefaults(const RoomType roomType)
{
    return RoomData(
        //TODO: Generate some name from a list
        "NEW ROOM",
        roomType == RoomType::TRIVIA_RUSH ? 5 : 2,
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
