#pragma once

#include <nlohmann/json.hpp>

struct RoomData;
class ProtocolPacketDeserializer
{
public:
    // Define as a static class (not a singleton):
    ProtocolPacketDeserializer() = delete;
    ProtocolPacketDeserializer(const ProtocolPacketDeserializer&) = delete;
    ProtocolPacketDeserializer& operator=(const ProtocolPacketDeserializer&) = delete;
    ProtocolPacketDeserializer(ProtocolPacketDeserializer&&) = delete;
    ProtocolPacketDeserializer& operator=(ProtocolPacketDeserializer&&) = delete;


    static RoomData deserialize(const nlohmann::json &data);


    /**
     * Parses the provided field from the data as an unsigned long.
     */
    static unsigned int toUL(int n);
};
