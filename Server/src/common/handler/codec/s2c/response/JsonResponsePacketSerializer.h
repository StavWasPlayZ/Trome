#pragma once

#include "ErrorResponse.h"

#include <nlohmann/json.hpp>

#include "Response.h"
#include "infrastructure/OBuffer.h"
#include "infrastructure/cryptoAlgorithm/ICryptoAlgorithm.h"

class ICryptoAlgorithm;


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
    static OBuffer serializeResponse(const ProtocolResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);


	/**
	 * Returns: The serialized object.
	 * 
	 * NOTE: The returned resource must be freed.
	 */
    static OBuffer serializeResponse(const LoginResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

	/**
	 * Returns: The serialized object.
	 * 
	 * NOTE: The returned resource must be freed.
	 */
    static OBuffer serializeResponse(const SignupResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

	/**
	 * Returns: The serialized object.
	 * 
	 * NOTE: The returned resource must be freed.
	 */
    static OBuffer serializeResponse(const ErrorResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const LogoutResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const JoinRoomResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const CreateRoomResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetRoomsResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetPlayersInRoomResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetHighScoresResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetUserStatisticsResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const CloseRoomResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const StartGameResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const LeaveRoomResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetRoomStateResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const UpdateRoomDataResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const LeaveGameResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetQuestionResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const SubmitAnswerResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetGameResultResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const AddQuestionResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const KickPlayerResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

private:
    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const QuestionResponse &response, const ICryptoAlgorithm& cryptoAlgorythm);

    static OBuffer serialize(ResponseCode msgCode, const nlohmann::json &data, const ICryptoAlgorithm& cryptoAlgorythm);

	/**
	 * Serializes the response into the provided json.
	 */
    static void serializeRegistrationResponseToJson(nlohmann::json &json, const RegistrationResponse &response);

};
