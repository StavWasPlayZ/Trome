#include "NotificationPacketSerializer.h"

#include "handler/codec/s2c/ProtocolPacketSerializer.h"

OBuffer NotificationPacketSerializer::serialize(const PlayerJoinedRoomNotification &notification)
{
    nlohmann::json data;

    data["player"] = ProtocolPacketSerializer::serializeAsJson(notification.player);

    return serialize(notification.id, data);
}

OBuffer NotificationPacketSerializer::serialize(const NotificationCode msgCode, const nlohmann::json &data)
{
    return ProtocolPacketSerializer::serialize(S2CPacketType::NOTIFICATION, static_cast<unsigned char>(msgCode), data);
}