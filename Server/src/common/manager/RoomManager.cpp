#include "RoomManager.h"

RoomManager::RoomManager(const IDatabase &database) :
    m_database(database)
{}

Room &RoomManager::createRoom(LoggedUser &admin, const RoomData &data)
{
    Room room(admin, data, this->m_database, RoomStatus::WAITING);
    const auto [entry, _] = this->m_rooms.emplace(room.getId(), room);

    return entry->second;
}

void RoomManager::deleteRoom(const int roomID)
{
    m_rooms.erase(roomID);
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

std::vector<Room*> RoomManager::getRooms() const
{
    std::vector<Room*> data;

    for (const auto& pair : this->m_rooms)
    {
        data.push_back(&pair.second);
    }

    return data;
}

std::optional<Room*> RoomManager::getRoom(const int roomID) const
{
    const auto it = m_rooms.find(roomID);
    if (it != m_rooms.end())
    {
        return &it->second;
    }

    return std::nullopt;
}

