namespace Trivia.Codec.S2C.Response;

public enum ResponseCode : byte
{
    Error = 0,
    Login,
    Signup,
    Logout,
    JoinRoom,
    CreateRoom,
    GetRooms,
    GetPlayersInRoom,
    GetHighScores,
    GetUserStatistics,
    CloseGame,
    StartGame,
    GetRoomState,
    LeaveRoom,
    UpdateRoomData,
    LeaveGame,
    GetQuestion,
    SubmitAnswer,
    GetGameResult,
    AddQuestion,
    KickPlayer
}