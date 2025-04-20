#pragma once

#include "JsonResponsePacketSerializer.h"

template <typename S>
 void JsonResponsePacketSerializer::serializeBaseResponseToJson(nlohmann::json& json, const ProtocolResponse<S>& response)
{
	json[ProtocolJsonKeys::STATUS] = response.status;
}

template <typename S>
void JsonResponsePacketSerializer::serializeRegistrationResponseToJson(nlohmann::json& json, const RegistrationResponse<S>& response)
{
	serializeBaseResponseToJson<S>(json, response);

	if (response.userId != -1)
	{
		json[ProtocolJsonKeys::USER_ID] = response.userId;
	}
}
