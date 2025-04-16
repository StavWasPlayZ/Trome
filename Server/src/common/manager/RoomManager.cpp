#include "RoomManager.h"

void RoomManager::createRoom(LoggedUser &admin, RoomData &data)
{
    Room room(data);
    room.addUser(admin);

    this->m_rooms.emplace(data.id, room);
}

void RoomManager::deleteRoom(const int roomID)
{
    auto it = m_rooms.find(roomID);
    if (it != m_rooms.end())
    {
        m_rooms.erase(it);
    }
}

RoomStatus RoomManager::getRoomStatus(const int roomID) const
{
    auto room = this->getRoom(roomID);

    if (!room.has_value())
    {
        return RoomStatus::NOT_FOUND;
    }
    return room.value().getData().status;
}

std::vector<RoomData> &RoomManager::getRooms() const
{
    std::vector<RoomData> data;

    for (const auto& pair : this->m_rooms)
    {
        Room room = pair.second;

        data.push_back(room.getData());
    }

    return data;
}

std::optional<Room &> RoomManager::getRoom(const int roomID) const
{
    auto it = m_rooms.find(roomID);
    if (it != m_rooms.end())
    {
        return it->second;
    }
    return std::nullopt; // room not found
}

