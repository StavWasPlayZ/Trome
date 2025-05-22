#pragma once

#include "handler/codec/s2c/response/Response.h"
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

private:
    static OBuffer serialize(ResponseCode msgCode, const nlohmann::json &data);
};
