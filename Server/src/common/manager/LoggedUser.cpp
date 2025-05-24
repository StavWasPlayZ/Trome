#include "LoggedUser.h"

#include "infrastructure/Server.h"

LoggedUser::LoggedUser(const unsigned int id, const std::string &username, Client &client) :
    m_client(client),
    m_id(id),
    m_username(username),
    m_currentRoom(nullptr)
{}

const std::string &LoggedUser::getUsername() const
{
    return this->m_username;
}

unsigned int LoggedUser::getId() const
{
    return this->m_id;
}

bool LoggedUser::operator==(const LoggedUser &other) const
{
    return this->getId() == other.getId();
}
Client &LoggedUser::getClient() const
{
    return this->m_client;
}

void LoggedUser::setCurrentRoom(Room &room)
{
    this->m_currentRoom = &room;
}

std::optional<Room *> LoggedUser::getCurrentRoom() const
{
    if (this->m_currentRoom == nullptr)
        return std::nullopt;

    return this->m_currentRoom;
}

void LoggedUser::removeFromRoom()
{
    this->m_currentRoom = nullptr;
}

void LoggedUser::handleDisconnecting()
{
    const std::optional<Room *> room = getCurrentRoom();

    if (room.has_value())
    {
        room.value()->removeUser(*this);
    }

    Server::getInstance().getLoginManager().logout(this->getClient());
}
