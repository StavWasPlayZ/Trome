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
{}

GetPlayersInRoomRequest::GetPlayersInRoomRequest(const unsigned int roomID) : roomID(roomID)
{}

ProtocolRequest *ProtocolRequest::fromRequest(const RequestInfo &info) {
    switch (info.id)
    {
    case RequestCode::LOGIN: return new LoginRequest(
        JsonRequestPacketDeserializer::deserializeLoginRequest(info.data)
    );
    case RequestCode::SIGNUP: return new SignupRequest(
        JsonRequestPacketDeserializer::deserializeSignupRequest(info.data)
    );
    case RequestCode::GET_PLAYERS_IN_ROOM: return new GetPlayersInRoomRequest(
        JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(info.data)
    );
    case RequestCode::JOIN_ROOM: return new JoinRoomRequest(
        JsonRequestPacketDeserializer::deserializeJoinRoomRequest(info.data)
    );
    case RequestCode::CREATE_ROOM: return new CreateRoomRequest(
        JsonRequestPacketDeserializer::deserializeCreateRoomRequest(info.data)
    );
    case RequestCode::GET_ROOMS: return new GetRoomsRequest(
        JsonRequestPacketDeserializer::deserializeGetRoomsRequest(info.data)
    );
    case RequestCode::GET_PLAYER_IN_ROOM: return new GetPlayersInRoomRequest(
        JsonRequestPacketDeserializer::deserializeGetPlayersInRoomRequest(info.data)
    );
    case RequestCode::GET_HIGH_SCORES: return new GetHighScoresRequest(
        JsonRequestPacketDeserializer::deserializeGetHighScoresRequest(info.data)
    );
    case RequestCode::GET_PERSONAL_STATISTICS: return new GetPersonalStatisticsRequest(
        JsonRequestPacketDeserializer::deserializeGetPersonalStatisticsRequest(info.data)
    );

    default: throw std::invalid_argument("Invalid request ID");
    }
}

GetRoomRequest::GetRoomRequest(const unsigned int roomID) : roomID(roomID)
{}

JoinRoomRequest::JoinRoomRequest(const unsigned int roomID) : roomID(roomID)
{}

CreateRoomRequest::CreateRoomRequest(const std::string &roomName, const unsigned int maxPlayers,
                                     const unsigned int questionCount, const unsigned int answerTimeout)
    : roomName(roomName), maxPlayers(maxPlayers), questionCount(questionCount), answerTimeout(answerTimeout)
{}
