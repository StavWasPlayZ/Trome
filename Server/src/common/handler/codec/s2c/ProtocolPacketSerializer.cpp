#include "ProtocolPacketSerializer.h"

#include "Constants.h"
#include "infrastructure/CommonCommunicator.h"

#include <cstring>

OBuffer ProtocolPacketSerializer::serialize(const S2CPacketType packetType, const ResponseCode msgCode, const nlohmann::json &data)
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
    writeBuffer[0] = static_cast<unsigned char>(msgCode);
    writeBuffer += SIZE_CODE;
    // JSON length
    writeInt(dataStr.size(), writeBuffer);
    writeBuffer += SIZE_JSON_LEN;

    // Actual JSON
    std::memcpy(writeBuffer, dataStr.c_str(), dataStr.size());

    return OBuffer(buffer, len);
}

void ProtocolPacketSerializer::writeInt(int num, unsigned char *const buffer)
{
    // Little/big endian format
    num = htonl(num);

    std::memcpy(buffer, &num, sizeof(int));
}