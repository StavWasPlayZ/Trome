#pragma once

#include <string>

enum class RoomStatus : unsigned int
{
    UNACTIVE = 0,
    ACTIVE = 1
};

struct RoomData
{
    unsigned int id;
    std::string name;
    unsigned int maxPlayers;
    unsigned int timePerQuestion;
    RoomStatus status;
};