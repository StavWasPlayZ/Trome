#include "Response.h"

ProtocolResponse::ProtocolResponse(const ResponseCode id) :
    id(id)
{}

ProtocolResponse::~ProtocolResponse() = default;

RegistrationResponse::RegistrationResponse(const ResponseCode id, const unsigned int userId) :
    ProtocolResponse(id),
    userId(userId)
{}

LoginResponse::LoginResponse(const unsigned int userId) :
    RegistrationResponse(ResponseCode::LOGIN, userId)
{}

SignupResponse::SignupResponse(const unsigned int userId) :
    RegistrationResponse(ResponseCode::SIGNUP, userId)
{}

LogoutResponse::LogoutResponse() :
    ProtocolResponse(ResponseCode::LOGOUT)
{}

JoinRoomResponse::JoinRoomResponse(const Room &room, const std::vector<LoggedUser*> &players) :
    ProtocolResponse(ResponseCode::JOIN_ROOM),
    room(room),
    players(players)
{}

CreateRoomResponse::CreateRoomResponse(const unsigned int roomId, const RoomData& data) :
    ProtocolResponse(ResponseCode::CREATE_ROOM),
    roomId(roomId),
    data(data)
{}

GetRoomsResponse::GetRoomsResponse(const std::vector<Room*> &rooms) :
    ProtocolResponse(ResponseCode::GET_ROOMS),
    rooms(rooms)
{}

GetPlayersInRoomResponse::GetPlayersInRoomResponse(const std::vector<LoggedUser*> &players) :
    ProtocolResponse(ResponseCode::GET_PLAYERS_IN_ROOM),
    players(players)
{}

GetHighScoresResponse::GetHighScoresResponse(const std::vector<std::pair<std::string, int>> &stats) :
    ProtocolResponse(ResponseCode::GET_HIGH_SCORES),
    stats(stats)
{}

GetPersonalStatisticsResponse::GetPersonalStatisticsResponse(const UserStatistics &stats) :
    ProtocolResponse(ResponseCode::GET_PERSONAL_STATISTICS),
    stats(stats)
{}

CloseRoomResponse::CloseRoomResponse() :
    ProtocolResponse(ResponseCode::CLOSE_ROOM)
{}

StartGameResponse::StartGameResponse() :
    ProtocolResponse(ResponseCode::START_GAME)
{}

LeaveRoomResponse::LeaveRoomResponse() :
    ProtocolResponse(ResponseCode::LEAVE_ROOM)
{}

GetRoomStateResponse::GetRoomStateResponse(const Room& room)
    : ProtocolResponse(ResponseCode::GET_ROOM_STATE), room(room)
{}

UpdateRoomDataResponse::UpdateRoomDataResponse() :
    ProtocolResponse(ResponseCode::UPDATE_ROOM_DATA)
{}

LeaveGameResponse::LeaveGameResponse() : 
    ProtocolResponse(ResponseCode::LEAVE_GAME)
{}

GetQuestionResponse::GetQuestionResponse(const int rotation) : 
    ProtocolResponse(ResponseCode::GET_QUESTION), rotation(rotation)
{}

SubmitAnswerResponse::SubmitAnswerResponse() : 
    ProtocolResponse(ResponseCode::SUBMIT_ANSWER)
{}

GetGameResultResponse::GetGameResultResponse(const std::vector<PlayerResult> results)
    : 
    ProtocolResponse(ResponseCode::GET_GAME_RESULT), 
    results(results)
{}
