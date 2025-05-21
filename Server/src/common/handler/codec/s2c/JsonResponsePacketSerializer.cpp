#include "JsonResponsePacketSerializer.h"

// For platform-correct network include
#include "infrastructure/Communicator.h"

#include "Constants.h"

#include <infrastructure/RoomData.h>

// fucking windows and their stupidass macros cost me 1 hour 30
#ifdef ERROR
#undef ERROR
#endif

OBuffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse &response)
{
	nlohmann::json data;
	serializeRegistrationResponseToJson(data, response);

	return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse &response)
{
	nlohmann::json data;
	serializeRegistrationResponseToJson(data, response);

	return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse &response)
{
    nlohmann::json data;

    data["req_code"] = response.reqCode;
    data["status"] = response.status;

    if (response.context.has_value())
    {
        data["context"] = response.context.value();
    }

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const LogoutResponse &response)
{
    return serializeJsonToProtocol(response.id, nlohmann::json::object());
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse &response)
{
    nlohmann::json data;

    data["room"] = serializeRoomToJson(response.room);

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse &response)
{
    nlohmann::json data;

    data["room_id"] = response.roomId;

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse &response)
{
    nlohmann::json data;

    nlohmann::json& rooms = data["rooms"] = nlohmann::json::array();

	for (const auto& room : response.rooms)
    {
        rooms.push_back(serializeRoomToJson(*room));
	}

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse &response)
{
    nlohmann::json data;

    data["players"] = nlohmann::json::array();

    for (const LoggedUser* user : response.players)
    {
        data["players"].push_back(serializePlayerToJson(*user));
    }

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetHighScoresResponse &response)
{
    nlohmann::json data;

    nlohmann::json scoresArr = nlohmann::json::array();

    for (const auto& [username, score] : response.stats)
    {
        scoresArr.push_back({
            {"username", username},
            {"score", score}
        });
    }

    data["high_scores"] = response.stats;

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatisticsResponse &response)
{

    nlohmann::json data;

    nlohmann::json stats;

    stats["points"] = response.stats.points;
    stats["games_played"] = response.stats.gamesPlayed;
    stats["questions_answered"] = response.stats.questionsAnswered;
    stats["questions_answered_correctly"] = response.stats.questionsAnsweredCorrect;
    stats["time_on_question_overall"] = response.stats.timeOnQuestionsOverall;
    stats["time_on_questions_avg"] = response.stats.timePerQuestionsAvg;

    data["stats"] = stats;

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse &response)
{
    return serializeJsonToProtocol(response.id, nlohmann::json::object());
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const StartGameResponse &response)
{
    return serializeJsonToProtocol(response.id, nlohmann::json::object());
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse &response)
{
    return serializeJsonToProtocol(response.id, nlohmann::json::object());
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse &response)
{
    nlohmann::json data;

    data["room"] = serializeRoomToJson(response.room);

    return serializeJsonToProtocol(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const UpdateRoomDataResponse &response)
{
    return serializeJsonToProtocol(response.id, nlohmann::json::object());
}

void JsonResponsePacketSerializer::serializeRegistrationResponseToJson(nlohmann::json &json,
                                                                       const RegistrationResponse &response)
{
    json["user_id"] = response.userId;
}

nlohmann::json JsonResponsePacketSerializer::serializePlayerToJson(const LoggedUser &player)
{
    nlohmann::json result;

    result["id"] = player.getId();
    result["username"] = player.getUsername();

    return result;
}

nlohmann::json JsonResponsePacketSerializer::serializeRoomToJson(const Room &room)
{
    nlohmann::json result;

    result["id"] = room.getId();
    result["status"] = room.getStatus();
    result["admin"] = serializePlayerToJson(room.getAdmin());

    result["players_count"] = room.getAllUsers().size();

    result["data"] = serializeRoomDataToJson(room.getData());

    return result;
}

nlohmann::json JsonResponsePacketSerializer::serializeRoomDataToJson(const RoomData &room)
{
    nlohmann::json result;

    result["name"] = room.name;
    result["max_players"] = room.maxPlayers;
    result["time_per_question_secs"] = room.timePerQuestionSecs;
    result["questions_count"] = room.questionsCount;

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
