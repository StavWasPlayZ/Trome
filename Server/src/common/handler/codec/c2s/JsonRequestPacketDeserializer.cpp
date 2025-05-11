#include "JsonRequestPacketDeserializer.h"

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const nlohmann::json &data)
{
	return LoginRequest(
		data.at("username"),
		data.at("password")
	);
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const nlohmann::json &data)
{
    return SignupRequest(
        data.at("username"),
        data.at("password"),
        data.at("email"),
        data.at("phone"),
        data.at("birthdate"),
        data.contains("address") ? std::optional(data.at("address")) : std::nullopt
    );
}

GetRoomsRequest JsonRequestPacketDeserializer::deserializeGetRoomsRequest(const nlohmann::json &)
{
    return GetRoomsRequest();
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(const nlohmann::json &data)
{
    return GetPlayersInRoomRequest(toUL(data.at("room_id")));
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const nlohmann::json &data)
{
    return JoinRoomRequest(toUL(data.at("room_id")));
}

GetHighScoresRequest JsonRequestPacketDeserializer::deserializeGetHighScoresRequest(const nlohmann::json &)
{
    return GetHighScoresRequest();
}

GetPersonalStatisticsRequest JsonRequestPacketDeserializer::deserializeGetPersonalStatisticsRequest(const nlohmann::json &)
{
    return GetPersonalStatisticsRequest();
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const nlohmann::json &data)
{
    return CreateRoomRequest(
		data.at("room_name"),
		data.at("max_players"),
		data.at("question_count"),
		data.at("answer_timeout")
	);
}

nlohmann::json JsonRequestPacketDeserializer::readJson(const unsigned char *data, const int jsonLen)
{
	// Avoid naughty buffer overflows
	if (jsonLen <= 0)
	{
        throw std::runtime_error("Invalid JSON length");
    }

	char* const jsonRaw = new char[jsonLen];
	std::memcpy(jsonRaw, data, jsonLen * sizeof(char));

    const nlohmann::json result = nlohmann::json::parse(std::string(jsonRaw, jsonLen));

    delete[] jsonRaw;
    return result;
}

unsigned int JsonRequestPacketDeserializer::toUL(const int n)
{
    if (n < 0)
    {
        throw std::runtime_error("Invalid room ID");
    }

    return static_cast<unsigned int>(n);
}
