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
	static LoginRequest deserializeLoginRequest(const unsigned char* data);

	/**
	* Returns: The deserialized object.
	*/
	static SignupRequest deserializeSignupRequest(const unsigned char* data);

private:
	static json deserializeProtocolMessage(const unsigned char* data);

	static json readJson(const unsigned char* data, const int jsonLen);
	static int readInt(const unsigned char* buffer);
};