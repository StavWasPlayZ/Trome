#pragma once

#include <string>

enum class RoomStatus : unsigned int
{
    WAITING = 0,
    PLAYING = 1,
    NOT_FOUND = 2
};

struct RoomData
{
    RoomData(
        const std::string &name,
        const RoomStatus &status,
        int maxPlayers,
        int timePerQuestion,
        int questionsCount
    );

    std::string name;
    unsigned int id;

    RoomStatus status;

    int maxPlayers;
    int secsPerQuestion;
    int questionsCount;

private:
    static unsigned int globalId;
};
