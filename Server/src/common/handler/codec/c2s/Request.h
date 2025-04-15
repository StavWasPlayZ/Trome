#pragma once

#include <string>
#include <optional>

#include "infrastructure/request/RequestInfo.h"


struct ProtocolRequest
{
	/**
	 * Constructs a new ProtocolRequest from the provided request info.
	 *
	 * NOTE: The returned resource must be freed.
	 */
	static ProtocolRequest* fromRequest(const RequestInfo& info);
};


struct LoginRequest : ProtocolRequest
{
	LoginRequest(const std::string& username, const std::string& password);
	
	const std::string username;
	const std::string password;
};

struct SignupRequest : LoginRequest
{
	SignupRequest(
		const std::string& username,
		const std::string& password,
		const std::string& email,
		const std::string& birthdate,
		const std::optional<std::string>& address
	);

	const std::string email;
	const std::string phone;
	const std::optional<std::string> address;
	const std::string birthdate;
};
