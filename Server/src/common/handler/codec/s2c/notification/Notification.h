#pragma once
#include "manager/LoggedUser.h"

enum class NotificationCode : unsigned char
{
    PLAYER_JOINED_ROOM
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
