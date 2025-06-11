#include "Response.h"

#include "infrastructure/Game.h"

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

GetHighScoresResponse::GetHighScoresResponse(const std::vector<std::pair<UserModel, int>> &stats) :
    ProtocolResponse(ResponseCode::GET_HIGH_SCORES),
    stats(stats)
{}

GetUserStatisticsResponse::GetUserStatisticsResponse(const UserStatistics &stats) :
    ProtocolResponse(ResponseCode::GET_USER_STATISTICS),
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

GetRoomStateResponse::GetRoomStateResponse(const Room& room) :
    ProtocolResponse(ResponseCode::GET_ROOM_STATE),
    room(room)
{}

UpdateRoomDataResponse::UpdateRoomDataResponse() :
    ProtocolResponse(ResponseCode::UPDATE_ROOM_DATA)
{}

LeaveGameResponse::LeaveGameResponse() :
    ProtocolResponse(ResponseCode::LEAVE_GAME)
{}

QuestionResponse::QuestionResponse(const ResponseCode id, const std::optional<UserQuestion> &question, const int points,
                                   const std::optional<std::vector<PlayerResult>> &results) :
    ProtocolResponse(id),
    question(question),
    points(points),
    results(results)
{}

GetQuestionResponse::GetQuestionResponse(const std::optional<UserQuestion> &question, const int points,
                                         const std::optional<std::vector<PlayerResult>> &results) :
    QuestionResponse(ResponseCode::GET_QUESTION, question, points, results)
{}

SubmitAnswerResponse::SubmitAnswerResponse(const std::optional<UserQuestion> &question, const int points,
                                           const std::optional<std::vector<PlayerResult>> &results) :
    QuestionResponse(ResponseCode::SUBMIT_ANSWER, question, points, results)
{}

GetGameResultResponse::GetGameResultResponse(const std::vector<PlayerResult>& results) :
    ProtocolResponse(ResponseCode::GET_GAME_RESULT), 
    results(results)
{}

AddQuestionResponse::AddQuestionResponse() : 
    ProtocolResponse(ResponseCode::ADD_QUESTION)
{}
