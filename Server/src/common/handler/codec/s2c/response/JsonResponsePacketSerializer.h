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
    static OBuffer serializeResponse(const ProtocolResponse &response, ICryptoAlgorithm& cryptoAlgorythm);


	/**
	 * Returns: The serialized object.
	 * 
	 * NOTE: The returned resource must be freed.
	 */
    static OBuffer serializeResponse(const LoginResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

	/**
	 * Returns: The serialized object.
	 * 
	 * NOTE: The returned resource must be freed.
	 */
    static OBuffer serializeResponse(const SignupResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

	/**
	 * Returns: The serialized object.
	 * 
	 * NOTE: The returned resource must be freed.
	 */
    static OBuffer serializeResponse(const ErrorResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const LogoutResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const JoinRoomResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const CreateRoomResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetRoomsResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetPlayersInRoomResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetHighScoresResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetUserStatisticsResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const CloseRoomResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const StartGameResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const LeaveRoomResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetRoomStateResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const UpdateRoomDataResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const LeaveGameResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetQuestionResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const SubmitAnswerResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const GetGameResultResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const AddQuestionResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const KickPlayerResponse &response, ICryptoAlgorithm &cryptoAlgorythm);

private:
    /**
     * Returns: The serialized object.
     *
     * NOTE: The returned resource must be freed.
     */
    static OBuffer serializeResponse(const QuestionResponse &response, ICryptoAlgorithm& cryptoAlgorythm);

    static OBuffer serialize(ResponseCode msgCode, const nlohmann::json &data, ICryptoAlgorithm& cryptoAlgorythm);

	/**
	 * Serializes the response into the provided json.
	 */
    static void serializeRegistrationResponseToJson(nlohmann::json &json, const RegistrationResponse &response);

};
