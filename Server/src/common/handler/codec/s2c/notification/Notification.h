#pragma once
#include "manager/LoggedUser.h"

struct RoomData;

enum class NotificationCode : unsigned char
{
    PLAYER_JOINED_ROOM,
    PLAYER_LEFT_ROOM,
    ROOM_CLOSED,
    ROOM_DATA_UPDATED
};


struct ProtocolNotification
{
    explicit ProtocolNotification(NotificationCode id);
    virtual ~ProtocolNotification();

    const NotificationCode id;
};


struct PlayerJoinedRoomNotification : ProtocolNotification
{
    explicit PlayerJoinedRoomNotification(const LoggedUser& player);

    const LoggedUser& player;
};

struct PlayerLeftRoomNotification : ProtocolNotification
{
    explicit PlayerLeftRoomNotification(unsigned int player);

    const unsigned int playerId;
};

struct RoomClosedNotification : ProtocolNotification
{
    RoomClosedNotification();
};

struct RoomDataUpdatedNotification : ProtocolNotification
{
    explicit RoomDataUpdatedNotification(const RoomData& data);

    const RoomData& data;
};
