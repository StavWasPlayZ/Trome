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

GetRoomRequest JsonRequestPacketDeserializer::deserializeGetRoomRequest(const nlohmann::json &data)
{
    const int roomID = data.at("roomID");

    if (roomID < 0)
    {
        throw std::runtime_error("Invalid room ID");
    }

    return GetRoomRequest(static_cast<unsigned int>(roomID));
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(const nlohmann::json &data)
{
    return GetPlayersInRoomRequest(data.at("roomID"));
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const nlohmann::json &data)
{
    return JoinRoomRequest(data.at("roomID"));
}

GetHighScoresRequest JsonRequestPacketDeserializer::deserializeGetHighScoresRequest(const nlohmann::json &)
{
    return GetHighScoresRequest();
}

GetPersonalStatisticsRequest JsonRequestPacketDeserializer::GetPersonalStatisticsRequest(const nlohmann::json &)
{
    return GetPersonalStatisticsRequest();
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const nlohmann::json &data)
{
    return CreateRoomRequest(
		data.at("roomName"),
		data.at("maxPlayers"),
		data.at("questionCount"),
		data.at("answerTime")
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

    return nlohmann::json::parse(std::string(jsonRaw, jsonLen));
}
