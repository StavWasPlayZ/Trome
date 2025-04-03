#pragma once

#include <nlohmann/json.hpp>

#include "Response.h"
#include "Constants.h"


class JsonResponsePacketSerializer
{
public:
	/**
	* Returns: The serialized object.
	* NOTE: The returned resource must be freed.
	*/
	static unsigned char* serializeResponse(const LoginResponse& response);

	/*/**
	* Returns: The serialized object.
	* NOTE: The returned resource must be freed.
	*/
	static unsigned char* serializeResponse(const SignupResponse& response);

	/**
	* Returns: The serialized object.
	* NOTE: The returned resource must be freed.
	*/
	static unsigned char* serializeResponse(const ErrorResponse& response);

private:
	/**
	 * Converts the provided json into a writable resource,
	 * prepending the necessary binary headers.
	 * 
	 * NOTE: The returned resource must be freed.
	 */
	static unsigned char* serializeJsonToProtocol(const ProtocolCode msgCode, const nlohmann::json data);

	static void writeInt(int num, unsigned char* const buffer);
};