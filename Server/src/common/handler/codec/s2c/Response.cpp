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

JoinRoomResponse::JoinRoomResponse(const ConsumingResponseStatus status) : ProtocolResponse(status)
{}

CreateRoomResponse::CreateRoomResponse(const GenericResponseStatus status, const unsigned int roomId) :
    ProtocolResponse(status),
    roomId(roomId)
{}

GetRoomsResponse::GetRoomsResponse(const GenericResponseStatus status, const std::vector<Room*> &rooms)
    : ProtocolResponse(status), rooms(rooms)
{}

GetPlayersInRoomResponse::GetPlayersInRoomResponse(const ConsumingResponseStatus status,
                                                   const std::optional<std::vector<LoggedUser*>> &players)
    : ProtocolResponse(status), players(players)
{}

GetHighScoresResponse::GetHighScoresResponse(const GeneralStatsStatus status, const std::vector<std::pair<std::string, int>> &stats)
    : ProtocolResponse(status), stats(stats)
{}

GetPersonalStatisticsResponse::GetPersonalStatisticsResponse(const GeneralStatsStatus status, const UserStatistics &stats)
    : ProtocolResponse(status), stats(stats)
{}
