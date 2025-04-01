#pragma once

#include "infrastructure/Responses.h"
#include "Constants.h"
#include "infrastructure/Communicator.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class JsonResponsePacketSerializer
{
public:
	/*
		Note: this function makes a new unsigned char*
		after using the buffer delete it
	*/
	static unsigned char* serializeLoginResponse(const LoginResponse& response)
	{
		json j = { {STATUS_KEY, response.status} };
		std::string str = j.dump();

		int len = CODE_SIZE + JSON_LEN_SIZE + str.size() + 1; // + 1 for null terminator
		unsigned char* buffer = new unsigned char[len];

		buffer[0] = LOGIN_CODE;
		JsonResponsePacketSerializer::writeIntToFourBytes(str.size() + 1, buffer);  // + 1 for null terminator

		memcpy(buffer, str.c_str(), len);

		return buffer;
	}

	/*
		Note: this function makes a new unsigned char*
		after using the buffer delete it
	*/
	static unsigned char* serializeSignupResponse(const SignupResponse& response)
	{
		json j = { {STATUS_KEY, response.status} };
		std::string str = j.dump();

		int len = CODE_SIZE + JSON_LEN_SIZE + str.size() + 1; // + 1 for null terminator
		unsigned char* buffer = new unsigned char[len];

		buffer[0] = SIGNUP_CODE;
		JsonResponsePacketSerializer::writeIntToFourBytes(str.size() + 1, buffer);  // + 1 for null terminator

		memcpy(buffer, str.c_str(), len);

		return buffer;
	}

	/*
		Note: this function makes a new unsigned char*
		after using the buffer delete it
	*/
	static unsigned char* serializeErrorResponse(const ErrorResponse& response)
	{
		json j = { {MESSAGE_KEY, response.message} };
		std::string str = j.dump();

		int len = CODE_SIZE + JSON_LEN_SIZE + str.size() + 1; // + 1 for null terminator
		unsigned char* buffer = new unsigned char[len];

		buffer[0] = ERROR_CODE;
		JsonResponsePacketSerializer::writeIntToFourBytes(str.size() + 1, buffer);  // + 1 for null terminator

		memcpy(buffer, str.c_str(), len);

		return buffer;
	}

private:
	static void writeIntToFourBytes(int num, unsigned char* buffer)
	{
		// those bit manipulation make the number into the correct format
		buffer[1] = (num >> 24) & 0xFF;
		buffer[2] = (num >> 16) & 0xFF;
		buffer[3] = (num >> 8) & 0xFF;
		buffer[4] = num & 0xFF;
	}
};