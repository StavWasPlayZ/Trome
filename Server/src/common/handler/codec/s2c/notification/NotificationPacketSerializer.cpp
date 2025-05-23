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

OBuffer NotificationPacketSerializer::serialize(const NotificationCode msgCode, const nlohmann::json &data)
{
    return ProtocolPacketSerializer::serialize(S2CPacketType::NOTIFICATION, static_cast<unsigned char>(msgCode), data);
}
