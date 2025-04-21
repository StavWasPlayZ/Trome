# pragma once

#include <unordered_map>
#include <optional>
#include "infrastructure/Room.h"

class RoomManager
{
public:
    RoomManager(const IDatabase& database);

    void createRoom(LoggedUser &admin, const RoomData &data);
    void deleteRoom(int roomID);

    RoomStatus getRoomStatus(int roomID) const;
    std::vector<RoomData*> getRooms() const;
    std::optional<Room*> getRoom(int roomID) const;

private:
    const IDatabase& m_database;

    /**
     * Room ID to room mappings.
     */
    std::unordered_map<unsigned int, Room&> m_rooms;
};
