#include "RoomManager.h"

RoomManager::RoomManager(const IDatabase &database) :
    m_database(database)
{}

Room &RoomManager::createRoom(LoggedUser &admin, const RoomData &data)
{
    const unsigned int roomId = Room::generateId();

    const auto [entry, _] = this->m_rooms.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(roomId),
        std::forward_as_tuple(roomId, admin, data, this->m_database, RoomStatus::WAITING)
    );

    return entry->second;
}

void RoomManager::deleteRoom(const Room& room)
{
    m_rooms.erase(room.getId());
}

RoomStatus RoomManager::getRoomStatus(const int roomID) const
{
    const auto room = this->getRoom(roomID);

    if (!room.has_value())
    {
        return RoomStatus::NOT_FOUND;
    }

    return room.value()->getStatus();
}

std::vector<Room*> RoomManager::getRooms()
{
    std::vector<Room*> data;

    for (auto& pair : this->m_rooms)
    {
        data.push_back(&pair.second);
    }

    return data;
}

std::optional<Room*> RoomManager::getRoom(const int roomID)
{
    const auto it = m_rooms.find(roomID);

    if (it != m_rooms.end())
    {
        return &it->second;
    }

    return std::nullopt;
}

std::vector<const Room*> RoomManager::getRooms() const
{
    std::vector<const Room*> data;

    for (const auto& pair : this->m_rooms)
    {
        data.push_back(&pair.second);
    }

    return data;
}

std::optional<const Room*> RoomManager::getRoom(const int roomID) const
{
    const auto it = m_rooms.find(roomID);

    if (it != m_rooms.end())
    {
        return &it->second;
    }

    return std::nullopt;
}

