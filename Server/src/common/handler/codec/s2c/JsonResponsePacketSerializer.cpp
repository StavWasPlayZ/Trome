#include "JsonResponsePacketSerializer.h"

// For platform-correct network include
#include "Constants.h"
#include "infrastructure/Communicator.h"

#include <infrastructure/RoomData.h>

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

    data["message"] = response.message;

    return serializeJsonToProtocol(ResponseCode::ERROR, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const LogoutResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<LogoutStatus>(data, response);

    return serializeJsonToProtocol(ResponseCode::LOGOUT, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<ConsumingResponseStatus>(data, response);

    return serializeJsonToProtocol(ResponseCode::JOIN_ROOM, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<GenericResponseStatus>(data, response);

    data["room_id"] = response.roomId;

    return serializeJsonToProtocol(ResponseCode::CREATE_ROOM, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse &response)
{

    nlohmann::json data;
    serializeBaseResponseToJson<GenericResponseStatus>(data, response);

    nlohmann::json rooms = nlohmann::json::array();

	for (const auto& room : response.rooms)
    {
        nlohmann::json roomObj;
	    const RoomData& roomData = room->getData();

		roomObj["id"] = roomData.id;
		roomObj["name"] = roomData.name;
        roomObj["max_players"] = roomData.maxPlayers;
        roomObj["status"] = roomData.status;
        roomObj["time_per_question"] = roomData.timePerQuestion;

	    // Also add the amount of players currently in the room.
	    roomObj["players_count"] = room->getAllUsers().size();

        rooms.push_back(roomObj);
	}

	data["rooms"] = rooms;

    return serializeJsonToProtocol(ResponseCode::GET_ROOMS, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<ConsumingResponseStatus>(data, response);

    if (response.players.has_value())
    {
        nlohmann::json players = nlohmann::json::array();

        for (const auto& player : response.players.value())
        {
            nlohmann::json playerObj;

            playerObj["id"] = player->getId();
            playerObj["username"] = player->getUsername();

            players.push_back(playerObj);
        }

        data["players"] = players;
    }

    return serializeJsonToProtocol(ResponseCode::GET_PLAYER_IN_ROOM, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetHighScoresResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<GeneralStatsStatus>(data, response);

    nlohmann::json scoresArr = nlohmann::json::array();

    for (const auto& [username, score] : response.stats)
    {
        scoresArr.push_back({
            {"username", username},
            {"score", score}
        });
    }

    data["highScores"] = response.stats;

    return serializeJsonToProtocol(ResponseCode::GET_HIGH_SCORES, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatisticsResponse &response)
{

    nlohmann::json data;
    serializeBaseResponseToJson<GeneralStatsStatus>(data, response);

    nlohmann::json stats;

    stats["points"] = response.stats.points;
    stats["games_played"] = response.stats.gamesPlayed;
    stats["questions_answered"] = response.stats.questionsAnswered;
    stats["questions_answered_correct"] = response.stats.questionsAnsweredCorrect;
    stats["time_on_questions_overall"] = response.stats.timeOnQuestionsOverall;
    stats["time_per_questions_avg"] = response.stats.timePerQuestionsAvg;

    data["personalStats"] = stats;

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
	writeBuffer[0] = static_cast<unsigned char>(msgCode);
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
