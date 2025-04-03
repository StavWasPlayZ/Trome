#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Request.h"
#include "Constants.h"

class JsonRequestPacketDeserializer
{
public:
	/**
	* Returns: The deserialized object.
	*/
	static LoginRequest deserializeLoginRequest(const unsigned char* data, const int jsonLen);

	/**
	* Returns: The deserialized object.
	*/
	static SignupRequest deserializeSignupRequest(const unsigned char* data, const int jsonLen);

private:
	static json readJson(const unsigned char* data, const int jsonLen);
	static int readInt(const unsigned char* buffer);
};