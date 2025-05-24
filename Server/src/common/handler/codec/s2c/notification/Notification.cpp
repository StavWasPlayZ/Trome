#include "Notification.h"

ProtocolNotification::ProtocolNotification(const NotificationCode id) : id(id)
{}

ProtocolNotification::~ProtocolNotification() = default;


PlayerJoinedRoomNotification::PlayerJoinedRoomNotification(const LoggedUser &player) :
    ProtocolNotification(NotificationCode::PLAYER_JOINED_ROOM),
    player(player)
{}

PlayerLeftRoomNotification::PlayerLeftRoomNotification(const unsigned int player) :
    ProtocolNotification(NotificationCode::PLAYER_LEFT_ROOM),
    playerId(player)
{}

RoomClosedNotification::RoomClosedNotification() :
    ProtocolNotification(NotificationCode::ROOM_CLOSED)
{}
