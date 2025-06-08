#pragma once

#include "S2CPacketType.h"

#include <nlohmann/json.hpp>

#include "infrastructure/OBuffer.h"
#include "infrastructure/Room.h"
#include "response/Response.h"

#include "infrastructure/cryptoAlgorithm/ICryptoAlgorithm.h"

class ICryptoAlgorithm;

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
    static OBuffer serialize(S2CPacketType packetType, unsigned char msgCode, const nlohmann::json &data, ICryptoAlgorithm& cryptoAlgorithm);


    static nlohmann::json serializeAsJson(const UserModel &player);
    //TODO: Remove when (if) LoggedUser is UserModel
    static nlohmann::json serializeAsJson(const LoggedUser &player);
    static nlohmann::json serializeAsJson(const Room &room);
    static nlohmann::json serializeAsJson(const RoomData &room);
    static nlohmann::json serializeAsJson(const PlayerResult &playerResult);
    static nlohmann::json serializeAsJson(const UserQuestion &question);

private:
    static void writeInt(int num, unsigned char* buffer);
};
