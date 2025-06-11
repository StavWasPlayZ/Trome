#include "Request.h"

#include "JsonRequestPacketDeserializer.h"
#include "infrastructure/RoomData.h"

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
{}

ProtocolRequest::~ProtocolRequest() = default;

ProtocolRequest *ProtocolRequest::fromRequest(const RequestInfo &info)
{
    return JsonRequestPacketDeserializer::deserialize(info);
}

JoinRoomRequest::JoinRoomRequest(const unsigned int roomId) : roomID(roomId)
{
}

GetUserStatisticsRequest::GetUserStatisticsRequest(const unsigned int userId) : userId(userId)
{
}

StartGameRequest::StartGameRequest(const RoomData &data) : data(data)
{
}

UpdateRoomDataRequest::UpdateRoomDataRequest(const RoomData &data) : data(data)
{
}

SubmitAnswerRequest::SubmitAnswerRequest(const int answer) : answer(answer)
{
}

KickPlayerRequest::KickPlayerRequest(unsigned int userId) : userId(userId)
{
}
