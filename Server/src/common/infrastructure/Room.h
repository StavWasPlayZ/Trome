#pragma once

#include <string>
#include <vector>
#include "manager/LoggedUser.h"

enum class RoomStatus : unsigned int
{
    WAITING = 0,
    PLAYING = 1
};

struct RoomData
{
    unsigned int id;
    std::string name;
    unsigned int maxPlayers;
    unsigned int timePerQuestion;
    RoomStatus status;
};

class Room
{
private:
    RoomData m_metadata;
    std::vector<LoggedUser&> m_users;

public:
    Room(const RoomData &data); // shouldn't get the users in c'tor, better to add them
    ~Room() = default;

    void addUser(const LoggedUser &user);
    void removeUser(const LoggedUser& user);
    std::vector<LoggedUser &> getAllUsers() const;
};