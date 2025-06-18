#include "RoomManager.h"

#include <algorithm>

RoomManager::RoomManager(const IDatabase &database) :
    m_database(database)
{}

Room &RoomManager::createRoom(LoggedUser &admin, const RoomType roomType, const RoomData &data)
{
    const long roomId = Room::generateId();

    const auto [entry, _] = this->m_rooms.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(roomId),
        std::forward_as_tuple(roomId, roomType, admin, data, this->m_database, RoomStatus::WAITING)
    );

    m_waitingRooms.emplace(roomId, &entry->second);
    return entry->second;
}

void RoomManager::deleteRoom(const Room &room)
{
    const long roomId = room.getId();

    m_rooms.erase(roomId);
    m_waitingRooms.erase(roomId);
}

void RoomManager::setRoomPlaying(Room &room)
{
    room.setStatus(RoomStatus::PLAYING);
    m_waitingRooms.erase(room.getId());
}

void RoomManager::setRoomWaiting(Room &room)
{
    room.setStatus(RoomStatus::WAITING);
    m_waitingRooms.emplace(room.getId(), &room);
}

std::vector<Room *> RoomManager::getWaitingRooms() const
{
    std::vector<Room *> results;

    for (const auto& pair : this->m_waitingRooms)
    {
        Room *room = pair.second;

        if (room->getAllUsers().size() < room->getData().maxPlayers)
        {
            results.push_back(room);
        }
    }

    return results;
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

int RoomManager::getQuestionCount() const
{
    return this->m_database.queryQuestionsCount();
}

