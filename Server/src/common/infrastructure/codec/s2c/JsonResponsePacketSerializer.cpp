#include "JsonResponsePacketSerializer.h"

unsigned char *JsonResponsePacketSerializer::serializeResponse(const LoginResponse &response)
{
	json data = {
		{ProtocolJsonKeys::STATUS, response.status}
	};

	return serializeJsonToProtocol(ProtocolCode::LOGIN, data);
}

unsigned char *JsonResponsePacketSerializer::serializeResponse(const SignupResponse &response)
{
	json data = {
		{ProtocolJsonKeys::STATUS, response.status}
	};

	return serializeJsonToProtocol(ProtocolCode::SIGNUP, data);
}

unsigned char *JsonResponsePacketSerializer::serializeResponse(const ErrorResponse &response)
{
	json data = {
		{ProtocolJsonKeys::MESSAGE, response.message}
	};

	return serializeJsonToProtocol(ProtocolCode::ERROR, data);
}

unsigned char *JsonResponsePacketSerializer::serializeJsonToProtocol(const ProtocolCode msgCode, const json data)
{
	const std::string str = data.dump();

	const int len = SIZE_CODE + SIZE_JSON_LEN + str.size() + 1; // + 1 for null terminator
	unsigned char* buffer = new unsigned char[len];

	buffer[0] = (unsigned char)msgCode;
	JsonResponsePacketSerializer::writeIntToFourBytes(str.size() + 1, buffer + 1);

	memcpy(buffer, str.c_str(), len);

	return buffer;
}

void JsonResponsePacketSerializer::writeIntToFourBytes(const int num, unsigned char *buffer)
{
	// Bit-shift the number into its respected bytes representation
	buffer[0] = (num >> 24) & 0xFF;
	buffer[1] = (num >> 16) & 0xFF;
	buffer[2] = (num >> 8) & 0xFF;
	buffer[3] = num & 0xFF;
}
