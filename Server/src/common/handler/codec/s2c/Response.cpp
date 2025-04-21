#include "Response.h"

LoginResponse::LoginResponse(const LoginStatus status, const unsigned int userId) : RegistrationResponse(status, userId)
{}

LoginResponse::LoginResponse(const LoginStatus status) : RegistrationResponse(status)
{}

SignupResponse::SignupResponse(const SignupStatus status, const unsigned int userId) : RegistrationResponse(status, userId)
{}

SignupResponse::SignupResponse(const SignupStatus status) : RegistrationResponse(status)
{}

LogoutResponse::LogoutResponse(const LogoutStatus status) : ProtocolResponse(status)
{}

ErrorResponse::ErrorResponse(const ErrorStatus status, const std::string &message) : ProtocolResponse(status),
    message(message)
{}

JoinRoomResponse::JoinRoomResponse(const GenericRoomResponseStatus status) : ProtocolResponse(status)
{}

CreateRoomResponse::CreateRoomResponse(const GenericRoomResponseStatus status, const unsigned int roomId) :
    ProtocolResponse(status),
    roomId(roomId)
{}

GetRoomsResponse::GetRoomsResponse(const GenericRoomResponseStatus status, const std::vector<RoomData*> &rooms)
    : ProtocolResponse(status), rooms(rooms)
{}

GetPlayersInRoomResponse::GetPlayersInRoomResponse(const GetPlayersInRoomStatus status,
                                                   const std::vector<std::string> &players)
    : ProtocolResponse(status), players(players)
{}

GetHighScoresResponse::GetHighScoresResponse(const GeneralStatsStatus status, const std::vector<std::pair<std::string, int>> &stats)
    : ProtocolResponse(status), stats(stats)
{}

GetPersonalStatisticsResponse::GetPersonalStatisticsResponse(const GeneralStatsStatus status, const UserStatistics &stats)
    : ProtocolResponse(status), stats(stats)
{}
