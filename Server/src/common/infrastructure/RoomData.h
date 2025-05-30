#pragma once

#include <chrono>
#include <string>

/**
 * Represents all fields of a Room customizable by its admin
 */
struct RoomData
{
    RoomData(
        const std::string &name,
        int maxPlayers,
        int timePerQuestion,
        int questionsCount
    );

    static RoomData ofDefaults();

    std::string name;

    int maxPlayers;
    int timePerQuestionSecs;
    int questionsCount;

    std::chrono::milliseconds getTimePerQuestionMs() const;
};
