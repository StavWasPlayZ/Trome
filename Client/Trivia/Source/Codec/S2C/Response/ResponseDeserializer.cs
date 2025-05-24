using Trivia.Codec.S2C.Response.Packets;

namespace Trivia.Codec.S2C.Response;

public static class ResponseDeserializer
{
    public static ProtocolResponse? Deserialize(ResponseCode code, string json)
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
            ResponseCode.GetPersonalStatistics => PacketDeserializer.Deserialize<GetPersonalStatisticsResponse>(json),
            ResponseCode.CloseGame => PacketDeserializer.Deserialize<CloseGameResponse>(json),
            ResponseCode.StartGame => PacketDeserializer.Deserialize<StartGameResponse>(json),
            ResponseCode.GetRoomState => PacketDeserializer.Deserialize<GetRoomStateResponse>(json),
            ResponseCode.LeaveRoom => PacketDeserializer.Deserialize<LeaveRoomResponse>(json),
            ResponseCode.UpdateRoomData => PacketDeserializer.Deserialize<UpdateRoomDataResponse>(json),
            _ => null
        };
    }
}