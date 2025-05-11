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