# pragma once

#include <optional>

#include <unordered_map>

#include "infrastructure/Room.h"

class RoomManager
{
public:
    explicit RoomManager(const IDatabase& database);

    Room& createRoom(LoggedUser &admin, const RoomData &data);
    void deleteRoom(const Room & room);


    void setRoomPlaying(Room& room);
    void setRoomWaiting(Room& room);

    std::vector<Room *> getWaitingRooms() const;


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

    /**
     * All rooms that are currently in a waiting state
     */
    std::unordered_map<unsigned int, Room *> m_waitingRooms;
};
