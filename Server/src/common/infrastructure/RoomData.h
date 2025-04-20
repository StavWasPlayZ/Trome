#include <string>

enum class RoomStatus : unsigned int
{
    WAITING = 0,
    PLAYING = 1,
    NOT_FOUND = 2
};

struct RoomData
{
    RoomData(const std::string &name, unsigned int id, const RoomStatus &status, unsigned int max_players,
             unsigned int time_per_question);

    std::string name;
    unsigned int id;

    RoomStatus status;

    unsigned int maxPlayers;
    unsigned int timePerQuestion;
};