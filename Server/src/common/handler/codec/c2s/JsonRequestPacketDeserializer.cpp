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
    std::optional<std::string> address = std::nullopt;

    if (data.contains("address"))
    {
        if (data.at("address").is_string())
        {
            address = std::optional(data.at("address"));
        }
    }

    return SignupRequest(
        data.at("username"),
        data.at("password"),
        data.at("email"),
        data.at("phone"),
        data.at("birthdate"),
        address
    );
}

LogoutRequest JsonRequestPacketDeserializer::deserializeLogoutRequest(const nlohmann::json &)
{
    return LogoutRequest();
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
    return CreateRoomRequest();
}

CloseRoomRequest JsonRequestPacketDeserializer::deserializeCloseRoomRequest(const nlohmann::json &)
{
    return CloseRoomRequest();
}

StartGameRequest JsonRequestPacketDeserializer::deserializeStartGameRequest(const nlohmann::json &)
{
    return StartGameRequest();
}

GetRoomStateRequest JsonRequestPacketDeserializer::deserializeGetRoomStateRequest(const nlohmann::json &)
{
    return GetRoomStateRequest();
}

LeaveRoomRequest JsonRequestPacketDeserializer::deserializeLeaveRoomRequest(const nlohmann::json &)
{
    return LeaveRoomRequest();
}

UpdateRoomDataRequest JsonRequestPacketDeserializer::deserializeUpdateRoomDataRequest(const nlohmann::json &data)
{
    return UpdateRoomDataRequest(
        RoomData(
            data.at("room_name"),
            data.at("max_players"),
            data.at("time_per_question"),
            data.at("questions_count")
        )
    );
}

nlohmann::json JsonRequestPacketDeserializer::readJson(const unsigned char *data, const int jsonLen)
{
	// Avoid naughty buffer overflows
	if (jsonLen <= 0)
	{
        throw std::runtime_error("Invalid JSON length: Parsing phase");
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
