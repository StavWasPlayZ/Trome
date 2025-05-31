#include "NotificationPacketSerializer.h"

#include "handler/codec/s2c/ProtocolPacketSerializer.h"

OBuffer NotificationPacketSerializer::serialize(const ProtocolNotification &notification)
{
    switch (notification.id)
    {
    case NotificationCode::PLAYER_JOINED_ROOM:
        return serialize(static_cast<const PlayerJoinedRoomNotification&>(notification));
    case NotificationCode::PLAYER_LEFT_ROOM:
        return serialize(static_cast<const PlayerLeftRoomNotification&>(notification));
    case NotificationCode::ROOM_CLOSED:
        return serialize(static_cast<const RoomClosedNotification&>(notification));
    case NotificationCode::ROOM_DATA_UPDATED:
        return serialize(static_cast<const RoomDataUpdatedNotification&>(notification));
    case NotificationCode::GAME_STARTED:
        return serialize(static_cast<const GameStartedNotification&>(notification));
    case NotificationCode::GAME_ENDED:
        return serialize(static_cast<const GameEndedNotification&>(notification));
    case NotificationCode::PLAYER_FINISHED:
        return serialize(static_cast<const PlayerFinishedNotification&>(notification));

    default: throw std::invalid_argument("Invalid notification ID");
    }
}


OBuffer NotificationPacketSerializer::serialize(const PlayerJoinedRoomNotification &notification)
{
    nlohmann::json data;

    data["player"] = ProtocolPacketSerializer::serializeAsJson(notification.player);

    return serialize(notification.id, data);
}

OBuffer NotificationPacketSerializer::serialize(const PlayerLeftRoomNotification &notification)
{
    nlohmann::json data;

    data["player_id"] = notification.playerId;

    return serialize(notification.id, data);
}

OBuffer NotificationPacketSerializer::serialize(const RoomClosedNotification &notification)
{
    return serialize(notification.id, nlohmann::json::object());
}

OBuffer NotificationPacketSerializer::serialize(const RoomDataUpdatedNotification &notification)
{
    nlohmann::json data;

    data["data"] = ProtocolPacketSerializer::serializeAsJson(notification.data);

    return serialize(notification.id, data);
}

OBuffer NotificationPacketSerializer::serialize(const GameStartedNotification &notification)
{
    nlohmann::json data;

    data["data"] = ProtocolPacketSerializer::serializeAsJson(notification.data);

    return serialize(notification.id, data);
}

OBuffer NotificationPacketSerializer::serialize(const GameEndedNotification &notification)
{
    nlohmann::json data;

    nlohmann::json &results = data["results"] = nlohmann::json::array();

    for (const PlayerResult& result : notification.results)
    {
        results.push_back(ProtocolPacketSerializer::serializeAsJson(result));
    }

    return serialize(notification.id, data);
}

OBuffer NotificationPacketSerializer::serialize(const PlayerFinishedNotification &notification)
{
    return serialize(notification.id, nlohmann::json::object());
}

OBuffer NotificationPacketSerializer::serialize(const NotificationCode msgCode, const nlohmann::json &data)
{
    return ProtocolPacketSerializer::serialize(S2CPacketType::NOTIFICATION, static_cast<unsigned char>(msgCode), data);
}
