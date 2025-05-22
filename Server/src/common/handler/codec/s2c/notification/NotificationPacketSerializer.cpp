#include "NotificationPacketSerializer.h"

#include "handler/codec/s2c/ProtocolPacketSerializer.h"

OBuffer NotificationPacketSerializer::serialize(const ResponseCode msgCode, const nlohmann::json &data)
{
    return ProtocolPacketSerializer::serialize(S2CPacketType::NOTIFICATION, msgCode, data);
}