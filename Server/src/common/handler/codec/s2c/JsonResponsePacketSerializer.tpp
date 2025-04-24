#pragma once

#include "Constants.h"
#include "JsonResponsePacketSerializer.h"

template <typename S>
 void JsonResponsePacketSerializer::serializeBaseResponseToJson(nlohmann::json& json, const ProtocolResponse<S>& response)
{
	json["status"] = response.status;
}

template <typename S>
void JsonResponsePacketSerializer::serializeRegistrationResponseToJson(nlohmann::json& json, const RegistrationResponse<S>& response)
{
	serializeBaseResponseToJson<S>(json, response);

	if (response.userId != -1)
	{
		json["user_id"] = response.userId;
	}
}
