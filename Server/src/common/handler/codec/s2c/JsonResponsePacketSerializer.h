#pragma once

#include <nlohmann/json.hpp>

#include "Response.h"
#include "Constants.h"

#include "infrastructure/OBuffer.h"


class JsonResponsePacketSerializer
{
public:
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
	static OBuffer serializeResponse(const ErrorResponse& response);

private:
	/**
	 * Converts the provided json into a writable resource,
	 * prepending the necessary binary headers.
	 * 
	 * NOTE: The returned resource must be freed.
	 */
	static OBuffer serializeJsonToProtocol(const ProtocolCode msgCode, const nlohmann::json data);

	static void writeInt(int num, unsigned char* const buffer);
};
