#include "JsonRequestPacketDeserializer.h"

// For platform-correct network include
#include "infrastructure/Communicator.h"

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
		data.at("email")
	);
}

// json JsonRequestPacketDeserializer::deserializeProtocolMessage(const unsigned char *data)
// {
//     // Skip the code (we already persumably know it if we're here)
// 	data += SIZE_CODE;

// 	const int jsonLen = readInt(data) * sizeof(char);
// 	data += SIZE_JSON_LEN;

// 	return readJson(data, jsonLen);
// }

nlohmann::json JsonRequestPacketDeserializer::readJson(const unsigned char *data, const int jsonLen)
{
	// Avoid naughty buffer overflows
	if (jsonLen <= 0)
	{
        throw std::runtime_error("Invalid JSON length");
    }

	std::string jsonRaw;
	std::memcpy(&jsonRaw, data, jsonLen * sizeof(char));

    return nlohmann::json::parse(jsonRaw);
}

int JsonRequestPacketDeserializer::readInt(const unsigned char *buffer)
{
	int result;
    std::memcpy(&result, buffer, sizeof(int));

	// Internet said to wrap result in this
	return ntohl(result);
}
