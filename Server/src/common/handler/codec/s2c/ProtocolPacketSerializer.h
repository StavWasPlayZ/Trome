#pragma once

#include "S2CPacketType.h"

#include <nlohmann/json.hpp>

#include "infrastructure/OBuffer.h"
#include "response/Response.h"

class ProtocolPacketSerializer
{
public:
    // Define as a static class:
    ProtocolPacketSerializer() = delete;
    ProtocolPacketSerializer(const ProtocolPacketSerializer&) = delete;
    ProtocolPacketSerializer& operator=(const ProtocolPacketSerializer&) = delete;
    ProtocolPacketSerializer(ProtocolPacketSerializer&&) = delete;
    ProtocolPacketSerializer& operator=(ProtocolPacketSerializer&&) = delete;

    /**
     * Converts the provided json into a writable resource,
     * prepending the necessary binary headers.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serialize(S2CPacketType packetType, ResponseCode msgCode, const nlohmann::json &data);

private:
    static void writeInt(int num, unsigned char* buffer);
};
