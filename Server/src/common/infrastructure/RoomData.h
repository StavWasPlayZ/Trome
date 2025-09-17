#pragma once

#include <chrono>
#include <string>

enum class RoomType;

/**
 * Represents all fields of a Room customizable by its admin
 */
struct RoomData
{
    RoomData(
        const std::string &name,
        int maxPlayers,
        int timePerQuestion,
        int questionsCount);

    static RoomData ofDefaults(RoomType roomType);

    std::string name;

    int maxPlayers;
    int timePerQuestionSecs;
    int questionsCount;

    std::chrono::milliseconds getTimePerQuestionMs() const;
};
