#pragma once
#include "infrastructure/Game.h"
#include "manager/LoggedUser.h"

struct PlayerResult;
enum class NotificationCode : unsigned char
{
    PLAYER_JOINED_ROOM,
    PLAYER_LEFT_ROOM,
    ROOM_CLOSED,
    ROOM_DATA_UPDATED,

    GAME_STARTED,
    GAME_ENDED,

    PLAYER_FINISHED
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

struct GameStartedNotification : ProtocolNotification
{
    explicit GameStartedNotification(const RoomData& data);

    const RoomData& data;
};

struct GameEndedNotification : ProtocolNotification
{
    explicit GameEndedNotification(const std::vector<PlayerResult> &results);

    const std::vector<PlayerResult>& results;
};

struct PlayerFinishedNotification : ProtocolNotification
{
    PlayerFinishedNotification();
};
