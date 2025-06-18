#include "Notification.h"

#include "infrastructure/PlayerResult.h"

ProtocolNotification::ProtocolNotification(const NotificationCode id) :
    id(id)
{}

ProtocolNotification::~ProtocolNotification() = default;


PlayerJoinedRoomNotification::PlayerJoinedRoomNotification(const LoggedUser &player) :
    ProtocolNotification(NotificationCode::PLAYER_JOINED_ROOM),
    player(player)
{}

PlayerLeftRoomNotification::PlayerLeftRoomNotification(const long playerId) :
    ProtocolNotification(NotificationCode::PLAYER_LEFT_ROOM),
    playerId(playerId)
{}

RoomClosedNotification::RoomClosedNotification() :
    ProtocolNotification(NotificationCode::ROOM_CLOSED)
{}

RoomDataUpdatedNotification::RoomDataUpdatedNotification(const RoomData &data) :
    ProtocolNotification(NotificationCode::ROOM_DATA_UPDATED),
    data(data)
{}

GameStartedNotification::GameStartedNotification(const RoomData &data) :
    ProtocolNotification(NotificationCode::GAME_STARTED),
    data(data)
{}

GameEndedNotification::GameEndedNotification(const std::vector<PlayerResult> &results) :
    ProtocolNotification(NotificationCode::GAME_ENDED), 
    results(results)
{}

PlayerFinishedNotification::PlayerFinishedNotification() : ProtocolNotification(NotificationCode::PLAYER_FINISHED)
{}

PlayerKickedNotification::PlayerKickedNotification(const long playerId) :
    ProtocolNotification(NotificationCode::PLAYER_KICKED),
    playerId(playerId)
{}
