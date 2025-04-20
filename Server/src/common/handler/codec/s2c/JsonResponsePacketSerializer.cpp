#include "JsonResponsePacketSerializer.h"

// For platform-correct network include
#include "infrastructure/Communicator.h"
#include <list>

// fucking windows and their stupid ass macros cost me 1 hour 30
#ifdef ERROR
#undef ERROR
#endif

OBuffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse &response)
{
	nlohmann::json data;
	serializeBaseResponseToJson<LoginStatus>(data, response);

	return serializeJsonToProtocol(ResponseCode::LOGIN, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse &response)
{
	nlohmann::json data;
	serializeBaseResponseToJson<SignupStatus>(data, response);

	return serializeJsonToProtocol(ResponseCode::SIGNUP, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse &response)
{
	nlohmann::json data;
	serializeBaseResponseToJson<ErrorStatus>(data, response);

	data[ProtocolJsonKeys::MESSAGE] = response.message;

	return serializeJsonToProtocol(ResponseCode::ERROR, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<GeneralRoomStatus>(data, response);

    return serializeJsonToProtocol(ResponseCode::JOIN_ROOM, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<GeneralRoomStatus>(data, response);

    return serializeJsonToProtocol(ResponseCode::CREATE_ROOM, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse &response)
{

    nlohmann::json data;
    std::list<nlohmann::json> rooms;
    serializeBaseResponseToJson<GeneralRoomStatus>(data, response);

	for (const auto& room : response.rooms)
    {
        nlohmann::json temp;

		temp["id"] = room.id;
		temp["name"] = room.name;
        temp["maxPlayers"] = room.maxPlayers;
        temp["status"] = room.status;
        temp["timePerQuestion"] = room.timePerQuestion;

        rooms.push_back(temp);
	}

	data["rooms"] = rooms;

    return serializeJsonToProtocol(ResponseCode::GET_ROOMS, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<GetPlayersInRoomStatus>(data, response);
    data["players"] = response.players;

    return serializeJsonToProtocol(ResponseCode::GET_PLAYER_IN_ROOM, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetHighScoresResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<GeneralStatsStatus>(data, response);
    data["highScores"] = response.stats;

    return serializeJsonToProtocol(ResponseCode::GET_HIGH_SCORES, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse &response)
{

    nlohmann::json data;
    serializeBaseResponseToJson<GeneralStatsStatus>(data, response);
    data["personalStats"] = response.stats;

    return serializeJsonToProtocol(ResponseCode::GET_PERSONAL_STATISTICS, data);
}

OBuffer JsonResponsePacketSerializer::serializeJsonToProtocol(const ResponseCode msgCode, const nlohmann::json &data)
{
	const std::string dataStr = data.dump();

	const int len = SIZE_CODE + SIZE_JSON_LEN + dataStr.size();
	unsigned char* const buffer = new unsigned char[len];

	unsigned char* writeBuffer = buffer;

	// Serializing:
	// Code
	writeBuffer[0] = (unsigned char)msgCode;
	writeBuffer += SIZE_CODE;
	// JSON length
	writeInt(dataStr.size(), writeBuffer);
	writeBuffer += SIZE_JSON_LEN;
	
	// Actual JSON
	std::memcpy(writeBuffer, dataStr.c_str(), dataStr.size());

	return OBuffer(buffer, len);
}

void JsonResponsePacketSerializer::writeInt(int num, unsigned char *const buffer)
{
	// Internet said to wrap value in this
	num = htonl(num);

	std::memcpy(buffer, &num, sizeof(int));
}
