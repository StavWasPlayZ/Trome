using Trivia.Codec.S2C.Response.Packets;
using Trivia.Codec.S2C.Response.Packets.Impl;

namespace Trivia.Codec.S2C.Response;

public static class ResponseDeserializer
{
    public static IProtocolResponse? Deserialize(ResponseCode code, string json)
    {
        return code switch
        {
            ResponseCode.Error => PacketDeserializer.Deserialize<ErrorResponse>(json),
            ResponseCode.Login => PacketDeserializer.Deserialize<LoginResponse>(json),
            ResponseCode.Signup => PacketDeserializer.Deserialize<SignupResponse>(json),
            ResponseCode.Logout => PacketDeserializer.Deserialize<LogoutResponse>(json),
            ResponseCode.JoinRoom => PacketDeserializer.Deserialize<JoinRoomResponse>(json),
            ResponseCode.CreateRoom => PacketDeserializer.Deserialize<CreateRoomResponse>(json),
            ResponseCode.GetRooms => PacketDeserializer.Deserialize<GetRoomsResponse>(json),
            ResponseCode.GetPlayersInRoom => PacketDeserializer.Deserialize<GetPlayersInRoomResponse>(json),
            ResponseCode.GetHighScores => PacketDeserializer.Deserialize<GetHighScoresResponse>(json),
            ResponseCode.GetUserStatistics => PacketDeserializer.Deserialize<GetUserStatisticsResponse>(json),
            ResponseCode.CloseGame => PacketDeserializer.Deserialize<CloseRoomResponse>(json),
            ResponseCode.StartGame => PacketDeserializer.Deserialize<StartGameResponse>(json),
            ResponseCode.GetRoomState => PacketDeserializer.Deserialize<GetRoomStateResponse>(json),
            ResponseCode.LeaveRoom => PacketDeserializer.Deserialize<LeaveRoomResponse>(json),
            ResponseCode.UpdateRoomData => PacketDeserializer.Deserialize<UpdateRoomDataResponse>(json),
            ResponseCode.LeaveGame => PacketDeserializer.Deserialize<LeaveGameResponse>(json),
            ResponseCode.GetGameResult => PacketDeserializer.Deserialize<GetGameResultResponse>(json),
            ResponseCode.GetQuestion => PacketDeserializer.Deserialize<GetQuestionResponse>(json),
            ResponseCode.SubmitAnswer => PacketDeserializer.Deserialize<SubmitAnswerResponse>(json),
            ResponseCode.AddQuestion => PacketDeserializer.Deserialize<AddQuestionResponse>(json),
            ResponseCode.KickPlayer => PacketDeserializer.Deserialize<KickPlayerResponse>(json),
            _ => null
        };
    }
}