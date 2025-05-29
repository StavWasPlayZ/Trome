#include "JsonResponsePacketSerializer.h"

// For platform-correct network include
#include "infrastructure/Communicator.h"

#include "handler/codec/s2c/ProtocolPacketSerializer.h"

// fucking windows and their stupidass macros cost me 1 hour 30
#ifdef ERROR
#undef ERROR
#endif

OBuffer JsonResponsePacketSerializer::serializeResponse(const ProtocolResponse &response)
{
    switch (response.id)
    {
    case ResponseCode::ERROR:
        return serializeResponse(static_cast<const ErrorResponse&>(response));
    case ResponseCode::LOGIN:
        return serializeResponse(static_cast<const LoginResponse&>(response));
    case ResponseCode::SIGNUP:
        return serializeResponse(static_cast<const SignupResponse&>(response));
    case ResponseCode::LOGOUT:
        return serializeResponse(static_cast<const LogoutResponse&>(response));
    case ResponseCode::JOIN_ROOM:
        return serializeResponse(static_cast<const JoinRoomResponse&>(response));
    case ResponseCode::CREATE_ROOM:
        return serializeResponse(static_cast<const CreateRoomResponse&>(response));
    case ResponseCode::GET_ROOMS:
        return serializeResponse(static_cast<const GetRoomsResponse&>(response));
    case ResponseCode::GET_PLAYERS_IN_ROOM:
        return serializeResponse(static_cast<const GetPlayersInRoomResponse&>(response));
    case ResponseCode::GET_HIGH_SCORES:
        return serializeResponse(static_cast<const GetHighScoresResponse&>(response));
    case ResponseCode::GET_PERSONAL_STATISTICS:
        return serializeResponse(static_cast<const GetPersonalStatisticsResponse&>(response));
    case ResponseCode::CLOSE_ROOM:
        return serializeResponse(static_cast<const CloseRoomResponse&>(response));
    case ResponseCode::START_GAME:
        return serializeResponse(static_cast<const StartGameResponse&>(response));
    case ResponseCode::GET_ROOM_STATE:
        return serializeResponse(static_cast<const GetRoomStateResponse&>(response));
    case ResponseCode::LEAVE_ROOM:
        return serializeResponse(static_cast<const LeaveRoomResponse&>(response));
    case ResponseCode::UPDATE_ROOM_DATA:
        return serializeResponse(static_cast<const UpdateRoomDataResponse&>(response));

    default: throw std::invalid_argument("Invalid response ID");
    }
}


OBuffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse &response)
{
	nlohmann::json data;
	serializeRegistrationResponseToJson(data, response);

	return serialize(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse &response)
{
	nlohmann::json data;
	serializeRegistrationResponseToJson(data, response);

	return serialize(response.id, data);
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

    return serialize(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const LogoutResponse &response)
{
    return serialize(response.id, nlohmann::json::object());
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse &response)
{
    nlohmann::json data;

    data["room"] = ProtocolPacketSerializer::serializeAsJson(response.room);

    data["players"] = nlohmann::json::array();
    for (const LoggedUser* user : response.players)
    {
        data["players"].push_back(ProtocolPacketSerializer::serializeAsJson(*user));
    }

    return serialize(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse &response)
{
    nlohmann::json data;

    data["room_id"] = response.roomId;
    data["data"] = ProtocolPacketSerializer::serializeAsJson(response.data);

    return serialize(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse &response)
{
    nlohmann::json data;

    nlohmann::json& rooms = data["rooms"] = nlohmann::json::array();

	for (const auto& room : response.rooms)
    {
        rooms.push_back(ProtocolPacketSerializer::serializeAsJson(*room));
	}

    return serialize(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse &response)
{
    nlohmann::json data;

    data["players"] = nlohmann::json::array();

    for (const LoggedUser* user : response.players)
    {
        data["players"].push_back(ProtocolPacketSerializer::serializeAsJson(*user));
    }

    return serialize(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetHighScoresResponse &response)
{
    nlohmann::json data;

    nlohmann::json scoresArr = nlohmann::json::array();

    for (const auto& [username, score] : response.stats)
    {
        scoresArr.push_back(
            {
                {"username", username},
                {"score", score}
            }
        );
    }

    data["high_scores"] = scoresArr;

    return serialize(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatisticsResponse &response)
{
    nlohmann::json data;

    nlohmann::json& stats = data["stats"] = nlohmann::json::object();

    stats["points"] = response.stats.points;
    stats["games_played"] = response.stats.gamesPlayed;
    stats["questions_answered"] = response.stats.questionsAnswered;
    stats["questions_answered_correctly"] = response.stats.questionsAnsweredCorrect;
    stats["time_on_question_overall"] = response.stats.timeOnQuestionsOverall;
    stats["time_on_questions_avg"] = response.stats.timePerQuestionsAvg;

    return serialize(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse &response)
{
    return serialize(response.id, nlohmann::json::object());
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const StartGameResponse &response)
{
    return serialize(response.id, nlohmann::json::object());
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse &response)
{
    return serialize(response.id, nlohmann::json::object());
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse &response)
{
    nlohmann::json data;

    data["room"] = ProtocolPacketSerializer::serializeAsJson(response.room);

    return serialize(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const UpdateRoomDataResponse &response)
{
    return serialize(response.id, nlohmann::json::object());
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const LeaveGameResponse &response)
{
    return serialize(response.id, nlohmann::json::object());
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetQuestionResponse &response)
{
    nlohmann::json data;

    data["question"] = ProtocolPacketSerializer::serializeAsJson(response.question, response.rotation);

    return serialize(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const SubmitAnswerResponse &response)
{
    nlohmann::json data;

    if (response.newQuestion.has_value())
    {
        data["new_question"] = ProtocolPacketSerializer::serializeAsJson(
            *response.newQuestion.value(),
            response.rotation
        );
    }

    return serialize(response.id, data);
}


OBuffer JsonResponsePacketSerializer::serializeResponse(const GetGameResultResponse &response)
{
    nlohmann::json data;
    nlohmann::json resultsArr = nlohmann::json::array();

    for (const auto& result : response.results)
    {
        resultsArr.push_back(ProtocolPacketSerializer::serializeAsJson(result));
    }

    data["results"] = resultsArr;

    return serialize(response.id, data);
}

OBuffer JsonResponsePacketSerializer::serialize(const ResponseCode msgCode, const nlohmann::json &data)
{
    return ProtocolPacketSerializer::serialize(S2CPacketType::RESPONSE, static_cast<unsigned char>(msgCode), data);
}


void JsonResponsePacketSerializer::serializeRegistrationResponseToJson(nlohmann::json &json,
                                                                       const RegistrationResponse &response)
{
    json["user_id"] = response.userId;
}
