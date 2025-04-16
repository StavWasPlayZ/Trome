# pragma once

#include <map>
#include <optional>
#include "infrastructure/Room.h"

class RoomManager
{
private:
    std::map<int, Room&> m_rooms;

public:
    RoomManager() = default;
    ~RoomManager() = default;

    void createRoom(LoggedUser &admin, RoomData &data);
    void deleteRoom(const int roomID);

    RoomStatus getRoomStatus(const int roomID) const;
    std::vector<RoomData>& getRooms() const;
    std::optional<Room &> getRoom(const int roomID) const;
};