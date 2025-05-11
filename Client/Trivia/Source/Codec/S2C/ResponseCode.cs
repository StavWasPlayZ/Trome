namespace Trivia.Codec.S2C;

public enum ResponseCode : byte
{
    Error = 0,
    Login,
    Signup,
    Logout,
    JoinRoom,
    CreateRoom,
    GetRooms,
    GetPlayerInRoom,
    GetHighScores,
    GetPersonalStatistics
}