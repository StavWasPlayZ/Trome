#include "Response.h"

LoginResponse::LoginResponse(const LoginStatus status, const unsigned int userId) :
    RegistrationResponse(ResponseCode::LOGIN, status, userId)
{}

LoginResponse::LoginResponse(const LoginStatus status) :
    LoginResponse(status, -1)
{}

SignupResponse::SignupResponse(const SignupStatus status, const unsigned int userId) :
    RegistrationResponse(ResponseCode::SIGNUP, status, userId),
    succeed(true),
    context("")
{}

SignupResponse::SignupResponse(const SignupStatus status, const std::string &context) :
    RegistrationResponse(ResponseCode::SIGNUP, status, -1),
    succeed(false),
    context(context)
{}

LogoutResponse::LogoutResponse(const LogoutStatus status) :
    ProtocolResponse(ResponseCode::LOGOUT, status)
{}

ErrorResponse::ErrorResponse(const ErrorStatus status, const std::string &message) :
    ProtocolResponse(ResponseCode::ERROR, status),
    message(message)
{}

JoinRoomResponse::JoinRoomResponse(const ConsumingResponseStatus status) :
    ProtocolResponse(ResponseCode::JOIN_ROOM, status)
{}

CreateRoomResponse::CreateRoomResponse(const GenericResponseStatus status, const unsigned int roomId) :
    ProtocolResponse(ResponseCode::CREATE_ROOM, status),
    roomId(roomId)
{}

GetRoomsResponse::GetRoomsResponse(const GenericResponseStatus status, const std::vector<Room*> &rooms) :
    ProtocolResponse(ResponseCode::GET_ROOMS, status),
    rooms(rooms)
{}

GetPlayersInRoomResponse::GetPlayersInRoomResponse(const ConsumingResponseStatus status,
                                                   const std::optional<std::vector<LoggedUser*>> &players) :
    ProtocolResponse(ResponseCode::GET_PLAYERS_IN_ROOM, status),
    players(players)
{}

GetHighScoresResponse::GetHighScoresResponse(const GeneralStatsStatus status, const std::vector<std::pair<std::string, int>> &stats) :
    ProtocolResponse(ResponseCode::GET_HIGH_SCORES, status),
    stats(stats)
{}

GetPersonalStatisticsResponse::GetPersonalStatisticsResponse(const GeneralStatsStatus status, const UserStatistics &stats) :
    ProtocolResponse(ResponseCode::GET_PERSONAL_STATISTICS, status),
    stats(stats)
{}

CloseRoomResponse::CloseRoomResponse(const GenericResponseStatus status) :
    ProtocolResponse(ResponseCode::CLOSE_ROOM, status)
{}

StartGameResponse::StartGameResponse(const GenericResponseStatus status) :
    ProtocolResponse(ResponseCode::START_GAME, status)
{}

LeaveRoomResponse::LeaveRoomResponse(const GenericResponseStatus status) :
    ProtocolResponse(ResponseCode::LEAVE_ROOM, status)
{}

GetRoomStateResponse::GetRoomStateResponse(const GenericResponseStatus protocolStatus, const Room& room)
    : ProtocolResponse(ResponseCode::GET_ROOM_STATE, protocolStatus), room(room)
{}

UpdateRoomDataResponse::UpdateRoomDataResponse(const GenericResponseStatus status) :
    ProtocolResponse(ResponseCode::UPDATE_ROOM_DATA, status)
{}
