#pragma once
#include "manager/LoggedUser.h"

enum class NotificationCode : unsigned char
{
    PLAYER_JOINED_ROOM,
    PLAYER_LEFT_ROOM
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
