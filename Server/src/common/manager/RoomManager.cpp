#include "RoomManager.h"

void RoomManager::createRoom(LoggedUser &admin, const RoomData &data)
{
    Room room(admin, data);
    this->m_rooms.emplace(data.id, room);
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

    return room.value()->getData().status;
}

std::vector<RoomData*> RoomManager::getRooms() const
{
    std::vector<RoomData*> data;

    for (const auto& pair : this->m_rooms)
    {
        data.push_back(&pair.second.getData());
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

