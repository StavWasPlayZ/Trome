#include "Room.h"

#include <list>

#include "db/IDatabase.h"

Room::Room(LoggedUser &admin, const RoomData &data, const IDatabase& database) :
    m_admin(&admin),
    m_metadata(data),
    m_database(database)
{
    // Add the admin to the room
    addUser(admin);
}

std::optional<Game *> Room::getCurrentGame() const
{
    return this->m_currentGame;
}

void Room::setCurrentGame(Game &game)
{
    this->m_currentGame = &game;
}

void Room::addUser(LoggedUser &user)
{
    this->m_users.push_back(&user);
}

void Room::removeUser(const LoggedUser &user)
{
    const auto it = std::ranges::find(this->m_users, &user);

    if (it != m_users.end())
    {
        m_users.erase(it);
    }
}

std::vector<LoggedUser*> Room::getAllUsers() const
{
    return this->m_users;
}

RoomData &Room::getData()
{
    return this->m_metadata;
}

LoggedUser &Room::getAdmin() const
{
    return *m_admin;
}

void Room::setAdmin(LoggedUser &admin)
{
    this->m_admin = &admin;
}
