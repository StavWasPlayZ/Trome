using System;
using System.Text;
using Newtonsoft.Json;
using Trivia.Codec.S2C.Response;

namespace Trivia.Codec.S2C;

public static class RequestPacketDeserializer
{
    /// <summary>
    /// T - Protocol Response
    /// </summary>
    public static ProtocolResponse? Deserialize(byte[] buffer)
    {
        var offset = 0;
        
        var code = (ResponseCode) buffer[offset];
        offset += CodecConstants.SizeCode;

        
        var rawJsonLen = buffer[offset..(offset + CodecConstants.SizeJsonLen)];
        if (BitConverter.IsLittleEndian)
        {
            Array.Reverse(rawJsonLen);
        }
        
        var jsonLen = BitConverter.ToInt32(rawJsonLen, 0);
        offset += CodecConstants.SizeJsonLen;
        
        
        var json = Encoding.UTF8.GetString(buffer, offset, jsonLen);

        return code switch
        {
            ResponseCode.Error => Deserialize<ErrorResponse>(json),
            ResponseCode.Login => Deserialize<LoginResponse>(json),
            ResponseCode.Signup => Deserialize<SignupResponse>(json),
            ResponseCode.Logout => Deserialize<LogoutResponse>(json),
            ResponseCode.JoinRoom => Deserialize<JoinRoomResponse>(json),
            ResponseCode.CreateRoom => Deserialize<CreateRoomResponse>(json),
            ResponseCode.GetRooms => Deserialize<GetRoomsResponse>(json),
            ResponseCode.GetPlayersInRoom => Deserialize<GetPlayersInRoomResponse>(json),
            ResponseCode.GetHighScores => Deserialize<GetHighScoresResponse>(json),
            ResponseCode.GetPersonalStatistics => Deserialize<GetPersonalStatisticsResponse>(json),
            ResponseCode.CloseGame => Deserialize<CloseGameResponse>(json),
            ResponseCode.StartGame => Deserialize<StartGameResponse>(json),
            ResponseCode.GetRoomState => Deserialize<GetRoomStateResponse>(json),
            ResponseCode.LeaveRoom => Deserialize<LeaveRoomResponse>(json),
            ResponseCode.UpdateRoomData => Deserialize<UpdateRoomDataResponse>(json),
            _ => null
        };
    }


    private static ProtocolResponse? Deserialize<T>(string json) where T : ProtocolResponse
    {
        return JsonConvert.DeserializeObject<T>(json, CodecConstants.JsonSerializerSettings);
    }
}