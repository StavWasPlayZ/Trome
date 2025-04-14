#pragma once

#include <nlohmann/json.hpp>

#include "Request.h"

class JsonRequestPacketDeserializer
{
public:
	static nlohmann::json readJson(const unsigned char* data, int jsonLen);

	/**
	* Returns: The deserialized object.
	*/
	static LoginRequest deserializeLoginRequest(const nlohmann::json& data);

	/**
	* Returns: The deserialized object.
	*/
	static SignupRequest deserializeSignupRequest(const nlohmann::json& data);
};