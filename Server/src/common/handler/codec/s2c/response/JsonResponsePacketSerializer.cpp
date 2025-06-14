#include "JsonResponsePacketSerializer.h"

// For platform-correct network include
#include "infrastructure/Communicator.h"

#include "handler/codec/s2c/ProtocolPacketSerializer.h"

#include "infrastructure/cryptoAlgorithm/ICryptoAlgorithm.h"

// fucking windows and their stupidass macros cost me 1 hour 30
#ifdef ERROR
#undef ERROR
#endif

OBuffer JsonResponsePacketSerializer::serializeResponse(const ProtocolResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    switch (response.id)
    {
    case ResponseCode::ERROR:
        return serializeResponse(static_cast<const ErrorResponse &>(response), cryptoAlgorythm);
    case ResponseCode::LOGIN:
        return serializeResponse(static_cast<const LoginResponse &>(response), cryptoAlgorythm);
    case ResponseCode::SIGNUP:
        return serializeResponse(static_cast<const SignupResponse &>(response), cryptoAlgorythm);
    case ResponseCode::LOGOUT:
        return serializeResponse(static_cast<const LogoutResponse &>(response), cryptoAlgorythm);
    case ResponseCode::JOIN_ROOM:
        return serializeResponse(static_cast<const JoinRoomResponse &>(response), cryptoAlgorythm);
    case ResponseCode::CREATE_ROOM:
        return serializeResponse(static_cast<const CreateRoomResponse &>(response), cryptoAlgorythm);
    case ResponseCode::GET_ROOMS:
        return serializeResponse(static_cast<const GetRoomsResponse &>(response), cryptoAlgorythm);
    case ResponseCode::GET_PLAYERS_IN_ROOM:
        return serializeResponse(static_cast<const GetPlayersInRoomResponse &>(response), cryptoAlgorythm);
    case ResponseCode::GET_HIGH_SCORES:
        return serializeResponse(static_cast<const GetHighScoresResponse &>(response), cryptoAlgorythm);
    case ResponseCode::GET_USER_STATISTICS:
        return serializeResponse(static_cast<const GetUserStatisticsResponse &>(response), cryptoAlgorythm);
    case ResponseCode::CLOSE_ROOM:
        return serializeResponse(static_cast<const CloseRoomResponse &>(response), cryptoAlgorythm);
    case ResponseCode::START_GAME:
        return serializeResponse(static_cast<const StartGameResponse &>(response), cryptoAlgorythm);
    case ResponseCode::GET_ROOM_STATE:
        return serializeResponse(static_cast<const GetRoomStateResponse &>(response), cryptoAlgorythm);
    case ResponseCode::LEAVE_ROOM:
        return serializeResponse(static_cast<const LeaveRoomResponse &>(response), cryptoAlgorythm);
    case ResponseCode::UPDATE_ROOM_DATA:
        return serializeResponse(static_cast<const UpdateRoomDataResponse &>(response), cryptoAlgorythm);
    case ResponseCode::LEAVE_GAME:
        return serializeResponse(static_cast<const LeaveGameResponse &>(response), cryptoAlgorythm);
    case ResponseCode::GET_QUESTION:
        return serializeResponse(static_cast<const GetQuestionResponse &>(response), cryptoAlgorythm);
    case ResponseCode::SUBMIT_ANSWER:
        return serializeResponse(static_cast<const SubmitAnswerResponse &>(response), cryptoAlgorythm);
    case ResponseCode::GET_GAME_RESULT:
        return serializeResponse(static_cast<const GetGameResultResponse &>(response), cryptoAlgorythm);
    case ResponseCode::ADD_QUESTION:
        return serializeResponse(static_cast<const AddQuestionResponse &>(response), cryptoAlgorythm);
    case ResponseCode::KICK_PLAYER:
        return serializeResponse(static_cast<const KickPlayerResponse &>(response), cryptoAlgorythm);

    default:
        throw std::invalid_argument("Invalid response ID");
    }
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    nlohmann::json data;
    serializeRegistrationResponseToJson(data, response);

    return serialize(response.id, data, cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    nlohmann::json data;
    serializeRegistrationResponseToJson(data, response);

    return serialize(response.id, data, cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    nlohmann::json data;

    data["req_code"] = response.reqCode;
    data["status"] = response.status;

    if (response.context.has_value())
    {
        data["context"] = response.context.value();
    }

    return serialize(response.id, data, cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const LogoutResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    return serialize(response.id, nlohmann::json::object(), cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    nlohmann::json data;

    data["room"] = ProtocolPacketSerializer::serializeAsJson(response.room);

    data["players"] = nlohmann::json::array();
    for (const LoggedUser *user : response.players)
    {
        data["players"].push_back(ProtocolPacketSerializer::serializeAsJson(*user));
    }

    return serialize(response.id, data, cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    nlohmann::json data;

    data["room_id"] = response.roomId;
    data["data"] = ProtocolPacketSerializer::serializeAsJson(response.data);

    return serialize(response.id, data, cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    nlohmann::json data;

    nlohmann::json &rooms = data["rooms"] = nlohmann::json::array();

    for (const Room *const room : response.rooms)
    {
        rooms.push_back(ProtocolPacketSerializer::serializeAsJson(*room));
    }

    return serialize(response.id, data, cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    nlohmann::json data;

    data["players"] = nlohmann::json::array();

    for (const LoggedUser *user : response.players)
    {
        data["players"].push_back(ProtocolPacketSerializer::serializeAsJson(*user));
    }

    return serialize(response.id, data, cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetHighScoresResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    nlohmann::json data;

    nlohmann::json scoresArr = nlohmann::json::array();

    for (const auto &[user, score] : response.stats)
    {
        scoresArr.push_back(
            {
                { "user", ProtocolPacketSerializer::serializeAsJson(user) },
                { "points", score }
            }
        );
    }

    data["high_scores"] = scoresArr;

    return serialize(response.id, data, cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetUserStatisticsResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    nlohmann::json data;

    nlohmann::json &stats = data["stats"] = nlohmann::json::object();

    stats["points"] = response.stats.points;
    stats["games_played"] = response.stats.gamesPlayed;
    stats["questions_answered"] = response.stats.questionsAnswered;
    stats["questions_answered_correctly"] = response.stats.questionsAnsweredCorrect;
    stats["time_on_question_overall"] = response.stats.timeOnQuestionsOverall;
    stats["time_on_questions_avg"] = response.stats.timePerQuestionsAvg;

    return serialize(response.id, data, cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    return serialize(response.id, nlohmann::json::object(), cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const StartGameResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    return serialize(response.id, nlohmann::json::object(), cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    return serialize(response.id, nlohmann::json::object(), cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    nlohmann::json data;

    data["room"] = ProtocolPacketSerializer::serializeAsJson(response.room);

    return serialize(response.id, data, cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const UpdateRoomDataResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    return serialize(response.id, nlohmann::json::object(), cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const LeaveGameResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    return serialize(response.id, nlohmann::json::object(), cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetQuestionResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    return serializeResponse(static_cast<const QuestionResponse &>(response), cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const SubmitAnswerResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    return serializeResponse(static_cast<const QuestionResponse &>(response), cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const GetGameResultResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    nlohmann::json data;

    nlohmann::json &resultsObj = data["results"] = nlohmann::json::array();

    for (const PlayerResult &result : response.results)
    {
        resultsObj.push_back(ProtocolPacketSerializer::serializeAsJson(result));
    }

    return serialize(response.id, data, cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const AddQuestionResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    return serialize(response.id, nlohmann::json::object(), cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const KickPlayerResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    return serialize(response.id, nlohmann::json::object(), cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const QuestionResponse &response,
                                                        const ICryptoAlgorithm &cryptoAlgorythm)
{
    nlohmann::json data;

    if (response.question.has_value())
    {
        data["question"] = ProtocolPacketSerializer::serializeAsJson(response.question.value());
    }

    data["points"] = response.points;

    if (response.results.has_value())
    {
        nlohmann::json &resultsObj = data["results"] = nlohmann::json::array();

        for (const PlayerResult &result : response.results.value())
        {
            resultsObj.push_back(ProtocolPacketSerializer::serializeAsJson(result));
        }
    }

    return serialize(response.id, data, cryptoAlgorythm);
}

OBuffer JsonResponsePacketSerializer::serialize(const ResponseCode msgCode, const nlohmann::json &data,
                                                const ICryptoAlgorithm &cryptoAlgorythm)
{
    return ProtocolPacketSerializer::serialize(
        S2CPacketType::RESPONSE,
        static_cast<unsigned char>(msgCode),
        data,
        cryptoAlgorythm
    );
}

void JsonResponsePacketSerializer::serializeRegistrationResponseToJson(nlohmann::json &json,
                                                                       const RegistrationResponse &response)
{
    json["user_id"] = response.userId;
}
