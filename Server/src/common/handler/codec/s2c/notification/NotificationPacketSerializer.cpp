#include "NotificationPacketSerializer.h"

#include "handler/codec/s2c/ProtocolPacketSerializer.h"

OBuffer NotificationPacketSerializer::serialize(const ProtocolNotification &notification, ICryptoAlgorithm& cryptoAlgorithm)
{
    switch (notification.id)
    {
    case NotificationCode::PLAYER_JOINED_ROOM:
        return serialize(static_cast<const PlayerJoinedRoomNotification&>(notification), cryptoAlgorithm);
    case NotificationCode::PLAYER_LEFT_ROOM:
        return serialize(static_cast<const PlayerLeftRoomNotification&>(notification), cryptoAlgorithm);
    case NotificationCode::ROOM_CLOSED:
        return serialize(static_cast<const RoomClosedNotification&>(notification), cryptoAlgorithm);
    case NotificationCode::ROOM_DATA_UPDATED:
        return serialize(static_cast<const RoomDataUpdatedNotification&>(notification), cryptoAlgorithm);
    case NotificationCode::GAME_STARTED:
        return serialize(static_cast<const GameStartedNotification&>(notification), cryptoAlgorithm);
    case NotificationCode::GAME_ENDED:
        return serialize(static_cast<const GameEndedNotification&>(notification), cryptoAlgorithm);
    case NotificationCode::PLAYER_FINISHED:
        return serialize(static_cast<const PlayerFinishedNotification&>(notification), cryptoAlgorithm);
    case NotificationCode::PLAYER_KICKED:
        return serialize(static_cast<const PlayerKickedNotification&>(notification), cryptoAlgorithm);

    default: throw std::invalid_argument("Invalid notification ID");
    }
}


OBuffer NotificationPacketSerializer::serialize(const PlayerJoinedRoomNotification &notification, ICryptoAlgorithm& cryptoAlgorithm)
{
    nlohmann::json data;

    data["player"] = ProtocolPacketSerializer::serializeAsJson(notification.player);

    return serialize(notification.id, data, cryptoAlgorithm);
}

OBuffer NotificationPacketSerializer::serialize(const PlayerLeftRoomNotification &notification, ICryptoAlgorithm& cryptoAlgorithm)
{
    nlohmann::json data;

    data["player_id"] = notification.playerId;

    return serialize(notification.id, data, cryptoAlgorithm);
}

OBuffer NotificationPacketSerializer::serialize(const RoomClosedNotification &notification, ICryptoAlgorithm& cryptoAlgorithm)
{
    return serialize(notification.id, nlohmann::json::object(), cryptoAlgorithm);
}

OBuffer NotificationPacketSerializer::serialize(const RoomDataUpdatedNotification &notification, ICryptoAlgorithm& cryptoAlgorithm)
{
    nlohmann::json data;

    data["data"] = ProtocolPacketSerializer::serializeAsJson(notification.data);

    return serialize(notification.id, data, cryptoAlgorithm);
}

OBuffer NotificationPacketSerializer::serialize(const GameStartedNotification &notification, ICryptoAlgorithm& cryptoAlgorithm)
{
    nlohmann::json data;

    data["data"] = ProtocolPacketSerializer::serializeAsJson(notification.data);

    return serialize(notification.id, data, cryptoAlgorithm);
}

OBuffer NotificationPacketSerializer::serialize(const GameEndedNotification &notification, ICryptoAlgorithm& cryptoAlgorithm)
{
    nlohmann::json data;

    nlohmann::json &results = data["results"] = nlohmann::json::array();

    for (const PlayerResult& result : notification.results)
    {
        results.push_back(ProtocolPacketSerializer::serializeAsJson(result));
    }

    return serialize(notification.id, data, cryptoAlgorithm);
}

OBuffer NotificationPacketSerializer::serialize(const PlayerFinishedNotification &notification, ICryptoAlgorithm& cryptoAlgorithm)
{
    return serialize(notification.id, nlohmann::json::object(), cryptoAlgorithm);
}

OBuffer NotificationPacketSerializer::serialize(const PlayerKickedNotification &notification, ICryptoAlgorithm& cryptoAlgorithm)
{
    nlohmann::json data;

    data["player_id"] = notification.playerId;

    return serialize(notification.id, data, cryptoAlgorithm);
}

OBuffer NotificationPacketSerializer::serialize(const NotificationCode msgCode, const nlohmann::json &data, ICryptoAlgorithm& cryptoAlgorithm)
{
    return ProtocolPacketSerializer::serialize(S2CPacketType::NOTIFICATION, static_cast<unsigned char>(msgCode), data, cryptoAlgorithm);
}
