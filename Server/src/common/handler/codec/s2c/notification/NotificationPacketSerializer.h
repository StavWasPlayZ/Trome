#pragma once

#include "Notification.h"
#include "infrastructure/OBuffer.h"

#include <nlohmann/json.hpp>

class NotificationPacketSerializer
{
public:
    // Define as a static class:
    NotificationPacketSerializer() = delete;
    NotificationPacketSerializer(const NotificationPacketSerializer&) = delete;
    NotificationPacketSerializer& operator=(const NotificationPacketSerializer&) = delete;
    NotificationPacketSerializer(NotificationPacketSerializer&&) = delete;
    NotificationPacketSerializer& operator=(NotificationPacketSerializer&&) = delete;


    static OBuffer serialize(const ProtocolNotification& notification);


    static OBuffer serialize(const PlayerJoinedRoomNotification& notification);


private:
    static OBuffer serialize(NotificationCode msgCode, const nlohmann::json &data);
};
