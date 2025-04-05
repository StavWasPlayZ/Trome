#include "JsonResponsePacketSerializer.h"

template <typename S>
inline nlohmann::json JsonResponsePacketSerializer::serializeResponseToJson(const RegistrationResponse<S> &response)
{
    nlohmann::json data = {
		{ProtocolJsonKeys::STATUS, response.status}
	};

	if (response.userId != -1)
	{
		data[ProtocolJsonKeys::USER_ID] = response.userId;
	}

	return data;
}
