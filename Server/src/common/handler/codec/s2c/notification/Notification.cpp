#include "Notification.h"

ProtocolNotification::ProtocolNotification(const NotificationCode id) : id(id)
{}

ProtocolNotification::~ProtocolNotification() = default;


PlayerJoinedRoomNotification::PlayerJoinedRoomNotification(const LoggedUser &player) :
    ProtocolNotification(NotificationCode::PLAYER_JOINED_ROOM),
    player(player)
{}
