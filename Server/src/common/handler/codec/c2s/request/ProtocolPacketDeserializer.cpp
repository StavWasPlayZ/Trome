#include "ProtocolPacketDeserializer.h"

#include "infrastructure/RoomData.h"

#include <stdexcept>

RoomData ProtocolPacketDeserializer::deserialize(const nlohmann::json &data)
{
    return RoomData(
        data.at("name"),
        data.at("max_players"),
        data.at("time_per_question_secs"),
        data.at("questions_count")
    );
}

unsigned int ProtocolPacketDeserializer::toUL(const int n)
{
    if (n < 0)
    {
        throw std::runtime_error("Negative ID provided");
    }

    return static_cast<unsigned int>(n);
}
