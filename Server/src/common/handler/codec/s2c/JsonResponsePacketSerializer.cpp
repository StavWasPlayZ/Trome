#include "JsonResponsePacketSerializer.h"

// For platform-correct network include
#include "infrastructure/Communicator.h"

// fucking windows and their stupid ass macros cost me 1 hour 30
#ifdef ERROR
#undef ERROR
#endif

OBuffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse &response)
{
	nlohmann::json data;

	serializeResponseToJson<LoginStatus>(
		data,
		(const RegistrationResponse<LoginStatus>&) response
	);

	return serializeJsonToProtocol(ProtocolCode::LOGIN, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse &response)
{
	nlohmann::json data;

	serializeResponseToJson<SignupStatus>(
		data,
		(const RegistrationResponse<SignupStatus>&) response
	);

	return serializeJsonToProtocol(ProtocolCode::SIGNUP, data);
}

OBuffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse &response)
{
	nlohmann::json data;

	serializeBaseResponseToJson<ErrorStatus>(
		data,
		(const ProtocolResponse<ErrorStatus>&) response
	);

	data[ProtocolJsonKeys::MESSAGE] = response.message;

	return serializeJsonToProtocol(ProtocolCode::ERROR, data);
}

OBuffer JsonResponsePacketSerializer::serializeJsonToProtocol(const ProtocolCode msgCode, const nlohmann::json data)
{
	const std::string dataStr = data.dump();

	const int len = SIZE_CODE + SIZE_JSON_LEN + dataStr.size();
	unsigned char* const buffer = new unsigned char[len];

	unsigned char* writeBuffer = buffer;

	// Serializing:
	// Code
	writeBuffer[0] = (unsigned char)msgCode;
	writeBuffer += SIZE_CODE;
	// JSON length
	JsonResponsePacketSerializer::writeInt(dataStr.size(), writeBuffer);
	writeBuffer += SIZE_JSON_LEN;
	
	// Actual JSON
	std::memcpy(writeBuffer, dataStr.c_str(), dataStr.size());

	return OBuffer(buffer, len);
}

void JsonResponsePacketSerializer::writeInt(int num, unsigned char *const buffer)
{
	// Internet said to wrap value in this
	num = htonl(num);

	std::memcpy(buffer, &num, sizeof(int));
}
