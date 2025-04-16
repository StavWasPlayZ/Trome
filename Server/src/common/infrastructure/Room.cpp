#include "Room.h"

#include <algorithm> // for std::find

// doesn't need to init the vector as it should be empty
Room::Room(const RoomData &data) : m_metadata(data)
{
}

void Room::addUser(LoggedUser &user)
{
    this->m_users.push_back(user);
}

void Room::removeUser(const LoggedUser &user)
{
    auto it = std::find(this->m_users.begin(), this->m_users.end(), user);
    if (it != m_users.end())
    {
        m_users.erase(it);
    }
}

std::vector<LoggedUser&> Room::getAllUsers() const
{
    return this->m_users;
}

RoomData Room::getData() const
{
    return this->m_metadata;
}
