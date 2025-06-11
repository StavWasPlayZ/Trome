#pragma once

#include <nlohmann/json.hpp>

#include "Request.h"

class JsonRequestPacketDeserializer
{
public:
    // Define as a static class (not a singleton):
    JsonRequestPacketDeserializer() = delete;
    JsonRequestPacketDeserializer(const JsonRequestPacketDeserializer&) = delete;
    JsonRequestPacketDeserializer& operator=(const JsonRequestPacketDeserializer&) = delete;
    JsonRequestPacketDeserializer(JsonRequestPacketDeserializer&&) = delete;
    JsonRequestPacketDeserializer& operator=(JsonRequestPacketDeserializer&&) = delete;


    static ProtocolRequest *deserialize(const RequestInfo &info);


	static nlohmann::json readJson(const unsigned char* data, int jsonLen);

	/**
	* Returns: The deserialized object.
	*/
	static LoginRequest deserializeLoginRequest(const nlohmann::json& data);

	/**
	* Returns: The deserialized object.
	*/
    static SignupRequest deserializeSignupRequest(const nlohmann::json &data);

    /**
    * Returns: The deserialized object.
    */
    static LogoutRequest deserializeLogoutRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static GetRoomsRequest deserializeGetRoomsRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static JoinRoomRequest deserializeJoinRoomRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static GetHighScoresRequest deserializeGetHighScoresRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static GetUserStatisticsRequest deserializeGetUserStatisticsRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static CreateRoomRequest deserializeCreateRoomRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static CloseRoomRequest deserializeCloseRoomRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static StartGameRequest deserializeStartGameRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static GetRoomStateRequest deserializeGetRoomStateRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static LeaveRoomRequest deserializeLeaveRoomRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static UpdateRoomDataRequest deserializeUpdateRoomDataRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static LeaveGameRequest deserializeLeaveGameRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static GetQuestionRequest deserializeGetQuestionRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static SubmitAnswerRequest deserializeSubmitAnswerRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static GetGameResultRequest deserializeGetGameResultRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static KickPlayerRequest deserializeKickPlayerRequest(const nlohmann::json &data);
};