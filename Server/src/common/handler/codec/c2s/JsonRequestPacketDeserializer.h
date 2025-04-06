#pragma once

#include <nlohmann/json.hpp>

#include "Request.h"
#include "Constants.h"

class JsonRequestPacketDeserializer
{
public:
	static nlohmann::json readJson(const unsigned char* data, const int jsonLen);

	/**
	* Returns: The deserialized object.
	*/
	static LoginRequest deserializeLoginRequest(const nlohmann::json& data);

	/**
	* Returns: The deserialized object.
	*/
	static SignupRequest deserializeSignupRequest(const nlohmann::json& data);
};