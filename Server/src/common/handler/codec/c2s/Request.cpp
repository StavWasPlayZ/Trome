#include "Request.h"

#include "JsonRequestPacketDeserializer.h"

LoginRequest::LoginRequest(const std::string &username, const std::string &password) :
    username(username),
    password(password)
{}

SignupRequest::SignupRequest(
    const std::string& username,
    const std::string& password,
    const std::string& email,
    const std::string& phone,
    const std::string& birthdate,
    const std::optional<std::string>& address
) : LoginRequest(username, password),
    email(email),
    phone(phone),
    address(address),
    birthdate(birthdate)
{
}

GetPlayersInRoomRequest::GetPlayersInRoomRequest(const unsigned int roomID) : roomID(roomID)
{
}

ProtocolRequest *ProtocolRequest::fromRequest(const RequestInfo &info) {
    switch (info.id)
    {
    case RequestCode::LOGIN: return new LoginRequest(JsonRequestPacketDeserializer::deserializeLoginRequest(info.data));
    case RequestCode::SIGNUP: return new SignupRequest(JsonRequestPacketDeserializer::deserializeSignupRequest(info.data));

    default: throw std::invalid_argument("Invalid request ID");
    }
}
