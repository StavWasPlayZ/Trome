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

	JsonResponsePacketSerializer::writeIntToBytes(str.size() + 1, buffer);
	std::memcpy(buffer + SIZE_CODE, str.c_str(), len);

	return buffer;
}

void JsonResponsePacketSerializer::writeIntToBytes(const int num, unsigned char *buffer)
{
	std::memcpy(buffer, &num, sizeof(num));
}
