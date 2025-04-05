#include "JsonResponsePacketSerializer.h"

template <typename S>
inline void JsonResponsePacketSerializer::serializeBaseResponseToJson(nlohmann::json& json, const ProtocolResponse<S>& response)
{
	json[ProtocolJsonKeys::STATUS] = response.status;
}

template <typename S>
inline void JsonResponsePacketSerializer::serializeResponseToJson(nlohmann::json& json, const RegistrationResponse<S>& response)
{
	serializeBaseResponseToJson<S>(json, response);

	if (response.userId != -1)
	{
		json[ProtocolJsonKeys::USER_ID] = response.userId;
	}
}
