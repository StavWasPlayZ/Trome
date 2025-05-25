#pragma once

#include "ErrorResponse.h"

#include <nlohmann/json.hpp>

#include "Response.h"
#include "infrastructure/OBuffer.h"


class JsonResponsePacketSerializer
{
public:
    // Define as a static class (not a singleton):
    JsonResponsePacketSerializer() = delete;
    JsonResponsePacketSerializer(const JsonResponsePacketSerializer&) = delete;
    JsonResponsePacketSerializer& operator=(const JsonResponsePacketSerializer&) = delete;
    JsonResponsePacketSerializer(JsonResponsePacketSerializer&&) = delete;
    JsonResponsePacketSerializer& operator=(JsonResponsePacketSerializer&&) = delete;


    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const ProtocolResponse& response);


	/**
	 * Returns: The serialized object.
	 * 
	 * NOTE: The returned resource must be freed.
	 */
	static OBuffer serializeResponse(const LoginResponse& response);

	/**
	 * Returns: The serialized object.
	 * 
	 * NOTE: The returned resource must be freed.
	 */
	static OBuffer serializeResponse(const SignupResponse& response);

	/**
	 * Returns: The serialized object.
	 * 
	 * NOTE: The returned resource must be freed.
	 */
    static OBuffer serializeResponse(const ErrorResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const LogoutResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const JoinRoomResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const CreateRoomResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetRoomsResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetPlayersInRoomResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetHighScoresResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetPersonalStatisticsResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const CloseRoomResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const StartGameResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const LeaveRoomResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetRoomStateResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const UpdateRoomDataResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const LeaveRoomResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetQuestionResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const SubmitAnswerResponse &response);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetGameResultResponse &response);

private:
    static OBuffer serialize(ResponseCode msgCode, const nlohmann::json &data);

	/**
	 * Serializes the response into the provided json.
	 */
    static void serializeRegistrationResponseToJson(nlohmann::json &json, const RegistrationResponse &response);

};
