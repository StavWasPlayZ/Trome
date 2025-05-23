#include "ProtocolPacketSerializer.h"

#include "Constants.h"

#include <cstring>
#include <netinet/in.h>

OBuffer ProtocolPacketSerializer::serialize(const S2CPacketType packetType, const unsigned char msgCode, const nlohmann::json &data)
{
    const std::string dataStr = data.dump();

    const int len = SIZE_PACKET_TYPE + SIZE_CODE + SIZE_JSON_LEN + dataStr.size();
    unsigned char* const buffer = new unsigned char[len];

    unsigned char* writeBuffer = buffer;

    // Serializing:

    // Packet Type
    writeBuffer[0] = static_cast<unsigned char>(packetType);
    writeBuffer += SIZE_PACKET_TYPE;
    // Code
    writeBuffer[0] = msgCode;
    writeBuffer += SIZE_CODE;
    // JSON length
    writeInt(dataStr.size(), writeBuffer);
    writeBuffer += SIZE_JSON_LEN;

    // Actual JSON
    std::memcpy(writeBuffer, dataStr.c_str(), dataStr.size());

    return OBuffer(buffer, len);
}


nlohmann::json ProtocolPacketSerializer::serializeAsJson(const LoggedUser &player)
{
    nlohmann::json result;

    result["id"] = player.getId();
    result["username"] = player.getUsername();

    return result;
}

nlohmann::json ProtocolPacketSerializer::serializeAsJson(const Room &room)
{
    nlohmann::json result;

    result["id"] = room.getId();
    result["status"] = room.getStatus();
    result["admin"] = serializeAsJson(room.getAdmin());

    result["players_count"] = room.getAllUsers().size();

    result["data"] = serializeAsJson(room.getData());

    return result;
}

nlohmann::json ProtocolPacketSerializer::serializeAsJson(const RoomData &room)
{
    nlohmann::json result;

    result["name"] = room.name;
    result["max_players"] = room.maxPlayers;
    result["time_per_question_secs"] = room.timePerQuestionSecs;
    result["questions_count"] = room.questionsCount;

    return result;
}


void ProtocolPacketSerializer::writeInt(int num, unsigned char *const buffer)
{
    // Little/big endian format
    num = htonl(num);

    std::memcpy(buffer, &num, sizeof(int));
}