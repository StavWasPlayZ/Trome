# pragma once

#include <map>
#include <optional>
#include "infrastructure/Room.h"

class RoomManager
{
private:
    std::map<int, Room> m_rooms;

public:
    RoomManager() = default;
    ~RoomManager() = default;

    void createRoom(const LoggedUser &admin, const RoomData &data);
    void deleteRoom(const int roomID);

    RoomStatus getRoomStatus(const int roomID);
    std::vector<RoomData> getRooms();
    std::optional<Room &> getRoom(const int roomID);
};