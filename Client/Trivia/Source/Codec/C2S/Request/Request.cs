namespace Trivia.Codec.C2S.Request;

public enum RequestCode : byte
{
    Logout = 0,
    Login = 1,
    Signup,
    GetPlayersInRoom,
    JoinRoom,
    CreateRoom,
    GetRooms,
    GetHighScores,
    GetPersonalStatistics
}

public abstract record ProtocolRequest(
    RequestCode Code
);

public record LoginRequest(
    string Username,
    string Password
) : ProtocolRequest(RequestCode.Login);

public record SignupRequest(
    string Username,
    string Password,
    string Email,
    string Phone,
    string? Address,
    string Birthday
) : ProtocolRequest(RequestCode.Signup);

public record GetPlayersInRoomRequest(
    int RoomId
) : ProtocolRequest(RequestCode.GetPlayersInRoom);

public record JoinRoomRequest(
    int RoomId
) : ProtocolRequest(RequestCode.JoinRoom);

public record GetRoomsRequest() 
    : ProtocolRequest(RequestCode.GetRooms);

public record CreateRoomRequest(
    string RoomName,
    string MaxPlayers,
    string QuestionCount,
    string AnswerTimeout
) : ProtocolRequest(RequestCode.CreateRoom);

public record GetHighScoresRequest()
    : ProtocolRequest(RequestCode.GetHighScores);

public record GetPersonalStatisticsRequest()
    : ProtocolRequest(RequestCode.GetPersonalStatistics);
