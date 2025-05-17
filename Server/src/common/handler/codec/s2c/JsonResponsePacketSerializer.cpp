#include "JsonResponsePacketSerializer.h"

// For platform-correct network include
#include "infrastructure/Communicator.h"

#include "Constants.h"

#include <infrastructure/RoomData.h>

// fucking windows and their stupid ass macros cost me 1 hour 30
#ifdef ERROR
#undef ERROR
#endif

OBuffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse &response)
{
	nlohmann::json data;
	serializeBaseResponseToJson<LoginStatus>(data, response);

	return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse &response)
{
	nlohmann::json data;
	serializeBaseResponseToJson<SignupStatus>(data, response);

    if (response.status != SignupStatus::SUCCESS)
    {
        data["context"] = response.context;
    }

	return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<ErrorStatus>(data, response);

    data["message"] = response.message;

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const LogoutResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<LogoutStatus>(data, response);

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<ConsumingResponseStatus>(data, response);

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<GenericResponseStatus>(data, response);

    data["room_id"] = response.roomId;

    return serializeJsonToProtocol(response.id, data);
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
        roomObj["time_per_question"] = roomData.secsPerQuestion;

	    // Also add the amount of players currently in the room.
	    roomObj["players_count"] = room->getAllUsers().size();

        rooms.push_back(roomObj);
	}

	data["rooms"] = rooms;

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<ConsumingResponseStatus>(data, response);

    if (response.players.has_value())
    {
        data["players"] = serializePlayersToJson(response.players.value());
    }

    return serializeJsonToProtocol(response.id, data);
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

    return serializeJsonToProtocol(response.id, data);
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

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<GenericResponseStatus>(data, response);

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const StartGameResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<GenericResponseStatus>(data, response);

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<GenericResponseStatus>(data, response);

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<GenericResponseStatus>(data, response);

    const Room& room = response.room;
    const RoomData& roomData = room.getData();

    data["questions_count"] = roomData.questionsCount;
    data["secs_per_question"] = roomData.secsPerQuestion;
    data["players"] = serializePlayersToJson(room.getAllUsers());
    data["is_game_running"] = roomData.status == RoomStatus::PLAYING;

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const UpdateRoomDataResponse &response)
{
    nlohmann::json data;
    serializeBaseResponseToJson<GenericResponseStatus>(data, response);

    return serializeJsonToProtocol(response.id, data);
}

nlohmann::json JsonResponsePacketSerializer::serializePlayersToJson(const std::vector<LoggedUser *> &players)
{
    nlohmann::json result = nlohmann::json::array();

    for (const auto& player : players)
    {
        nlohmann::json playerObj;

        playerObj["id"] = player->getId();
        playerObj["username"] = player->getUsername();

        result.push_back(playerObj);
    }

    return result;
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
