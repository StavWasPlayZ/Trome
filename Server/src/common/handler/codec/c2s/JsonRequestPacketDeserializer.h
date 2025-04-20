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
    static GetPlayersInRoomRequest deserializeGetPlayersInRoomRequest(const nlohmann::json &data);

    /**
     * Returns: The deserialized object.
     */
    static JoinRoomRequest deserializeJoinRoomRequest(const nlohmann::json &data);
};