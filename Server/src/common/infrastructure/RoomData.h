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
    RoomData::RoomData(
        const std::string &name,
        const RoomStatus &status,
        unsigned int max_players,
        unsigned int time_per_question
    );

    std::string name;
    unsigned int id;

    RoomStatus status;

    unsigned int maxPlayers;
    unsigned int timePerQuestion;

private:
    static unsigned int globalId;
};
