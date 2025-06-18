#include "JsonRequestPacketDeserializer.h"

#include "ProtocolPacketDeserializer.h"

#include <iostream>

ProtocolRequest *JsonRequestPacketDeserializer::deserialize(const RequestInfo &info)
{
    switch (info.id)
    {
    case RequestCode::LOGIN: return new LoginRequest(
        deserializeLoginRequest(info.data)
    );
    case RequestCode::SIGNUP: return new SignupRequest(
        deserializeSignupRequest(info.data)
    );
    case RequestCode::LOGOUT: return new LogoutRequest(
        deserializeLogoutRequest(info.data)
    );
    case RequestCode::GET_PLAYERS_IN_ROOM: return new GetPlayersInRoomRequest(
        deserializeGetPlayersInRoomRequest(info.data)
    );
    case RequestCode::JOIN_ROOM: return new JoinRoomRequest(
        deserializeJoinRoomRequest(info.data)
    );
    case RequestCode::CREATE_ROOM: return new CreateRoomRequest(
        deserializeCreateRoomRequest(info.data)
    );
    case RequestCode::GET_ROOMS: return new GetRoomsRequest(
        deserializeGetRoomsRequest(info.data)
    );
    case RequestCode::GET_HIGH_SCORES: return new GetHighScoresRequest(
        deserializeGetHighScoresRequest(info.data)
    );
    case RequestCode::GET_USER_STATISTICS: return new GetUserStatisticsRequest(
        deserializeGetUserStatisticsRequest(info.data)
    );
    case RequestCode::CLOSE_ROOM: return new CloseRoomRequest(
        deserializeCloseRoomRequest(info.data)
    );
    case RequestCode::START_GAME:return new StartGameRequest(
        deserializeStartGameRequest(info.data)
    );
    case RequestCode::GET_ROOM_STATE: return new GetRoomStateRequest(
        deserializeGetRoomStateRequest(info.data)
    );
    case RequestCode::LEAVE_ROOM: return new LeaveRoomRequest(
        deserializeLeaveRoomRequest(info.data)
    );
    case RequestCode::UPDATE_ROOM_DATA: return new UpdateRoomDataRequest(
        deserializeUpdateRoomDataRequest(info.data)
    );
    case RequestCode::LEAVE_GAME: return new LeaveGameRequest(
        deserializeLeaveGameRequest(info.data)
    );
    case RequestCode::GET_QUESTION: return new GetQuestionRequest(
        deserializeGetQuestionRequest(info.data)
    );
    case RequestCode::SUBMIT_ANSWER: return new SubmitAnswerRequest(
        deserializeSubmitAnswerRequest(info.data)
    );
    case RequestCode::GET_GAME_RESULT: return new GetGameResultRequest(
        deserializeGetGameResultRequest(info.data)
    );
    case RequestCode::ADD_QUESTION: return new AddQuestionRequest(
        deserializeAddQuestionRequest(info.data)
    );
    case RequestCode::KICK_PLAYER: return new KickPlayerRequest(
        deserializeKickPlayerRequest(info.data)
    );

    default:
        throw std::invalid_argument("Invalid request ID");
    }
}

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

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(const nlohmann::json &)
{
    // return GetPlayersInRoomRequest(ProtocolPacketDeserializer::toUL(data.at("room_id")));
    return GetPlayersInRoomRequest();
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const nlohmann::json &data)
{
    return JoinRoomRequest(data.at("room_id"));
}

GetHighScoresRequest JsonRequestPacketDeserializer::deserializeGetHighScoresRequest(const nlohmann::json &)
{
    return GetHighScoresRequest();
}

GetUserStatisticsRequest JsonRequestPacketDeserializer::deserializeGetUserStatisticsRequest(const nlohmann::json &data)
{
    return GetUserStatisticsRequest(data.at("user_id"));
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const nlohmann::json &data)
{
    return CreateRoomRequest(data.at("room_type"));
}

CloseRoomRequest JsonRequestPacketDeserializer::deserializeCloseRoomRequest(const nlohmann::json &)
{
    return CloseRoomRequest();
}

StartGameRequest JsonRequestPacketDeserializer::deserializeStartGameRequest(const nlohmann::json &data)
{
    return StartGameRequest(
        ProtocolPacketDeserializer::deserialize(data.at("data"))
    );
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
        ProtocolPacketDeserializer::deserialize(data.at("data"))
    );
}

LeaveGameRequest JsonRequestPacketDeserializer::deserializeLeaveGameRequest(const nlohmann::json &)
{
    return LeaveGameRequest();
}

GetQuestionRequest JsonRequestPacketDeserializer::deserializeGetQuestionRequest(const nlohmann::json &)
{
    return GetQuestionRequest();
}

SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(const nlohmann::json &data)
{
    return SubmitAnswerRequest(data.at("answer"));
}

GetGameResultRequest JsonRequestPacketDeserializer::deserializeGetGameResultRequest(const nlohmann::json &)
{
    return GetGameResultRequest();
}

AddQuestionRequest JsonRequestPacketDeserializer::deserializeAddQuestionRequest(const nlohmann::json &data)
{
    return AddQuestionRequest(
        Question(
            data.at("question"),
            data.at("answers")
        )
    );
}

KickPlayerRequest JsonRequestPacketDeserializer::deserializeKickPlayerRequest(const nlohmann::json &data)
{
    return KickPlayerRequest(
        data.at("user_id")
    );
}

nlohmann::json JsonRequestPacketDeserializer::readJson(const unsigned char *data, const int jsonLen,
                                                       const ICryptoAlgorithm &cryptoAlgorithm)
{
    // Avoid naughty buffer overflows
    if (jsonLen <= 0)
    {
        throw std::runtime_error("Invalid JSON length: Parsing phase");
    }

    char *const jsonRaw = new char[jsonLen];
    std::memcpy(jsonRaw, data, jsonLen * sizeof(char));

    const nlohmann::json result = nlohmann::json::parse(
        cryptoAlgorithm.decrypt(
            std::string(jsonRaw, jsonLen)
        )
    );

    delete[] jsonRaw;
    return result;
}
