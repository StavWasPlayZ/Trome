#include "JsonResponsePacketSerializer.h"

// For platform-correct network include
#include "infrastructure/Communicator.h"

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
	unsigned char* const buffer = new unsigned char[len];

	unsigned char* writeBuffer = buffer;

	// Serializing:
	// Code
	writeBuffer[0] = (unsigned char)msgCode;
	writeBuffer += SIZE_CODE;
	// JSON length
	JsonResponsePacketSerializer::writeInt(str.size() + 1, writeBuffer);
	writeBuffer += SIZE_JSON_LEN;
	// Actual JSON
	std::memcpy(writeBuffer + SIZE_CODE, str.c_str(), len);

	return buffer;
}

void JsonResponsePacketSerializer::writeInt(int num, unsigned char *const buffer)
{
	// Internet said to wrap value in this
	num = htonl(num);

	std::memcpy(buffer, &num, sizeof(int));
}
