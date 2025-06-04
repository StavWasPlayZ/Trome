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

// GetPlayersInRoomRequest::GetPlayersInRoomRequest(const unsigned int roomID) : roomID(roomID)
// {
// }

ProtocolRequest::~ProtocolRequest() = default;

ProtocolRequest *ProtocolRequest::fromRequest(const RequestInfo &info) {
    return JsonRequestPacketDeserializer::deserialize(info);
}

JoinRoomRequest::JoinRoomRequest(const unsigned int roomID) : roomID(roomID)
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

AddQuestionRequest::AddQuestionRequest(const std::string &question, const std::vector<std::string> &answers) : question(question), answers(answers)
{
}
