namespace Trivia.Codec.C2S;

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
    GetPersonalStatistics,
    CloseGame,
    StartGame,
    GetRoomState,
    LeaveRoom,
    UpdateRoomData
}