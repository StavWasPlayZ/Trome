# pragma once

#include <unordered_map>
#include <optional>
#include "infrastructure/Room.h"

class RoomManager
{
public:
    explicit RoomManager(const IDatabase& database);

    Room& createRoom(LoggedUser &admin, const RoomData &data);
    void deleteRoom(int roomID);

    RoomStatus getRoomStatus(int roomID) const;
    std::vector<Room*> getRooms();
    std::optional<Room*> getRoom(int roomID);

    std::vector<const Room*> getRooms() const;
    std::optional<const Room*> getRoom(int roomID) const;

private:
    const IDatabase& m_database;

    /**
     * Room ID to room mappings.
     */
    std::unordered_map<unsigned int, Room> m_rooms;
};
