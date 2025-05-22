using System;
using System.Diagnostics;
using Newtonsoft.Json;
using Trivia.Codec.S2C.Notification;
using Trivia.Codec.S2C.Response;

namespace Trivia.Codec.S2C;

public static class PacketDeserializer
{    
    private static ProtocolResponse? DeserializeResponse(ResponseCode code, string json)
    {
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

    private static ProtocolNotification? DeserializeNotification(ResponseCode code, string json)
    {
        return code switch
        {
            _ => null
        };
    }
    
    
    public static S2CPacket? Deserialize(S2CPacketType packetType, byte code, string json)
    {
        S2CPacket? result;

        switch (packetType)
        {
            case S2CPacketType.Response:
                VerboseLog($"Successfully received response of code {code}: {json}");
                result = DeserializeResponse((ResponseCode) code, json);
                break;
            
            case S2CPacketType.Notification:
                VerboseLog($"Successfully received notification of code {code}: {json}");
                result = DeserializeNotification((ResponseCode) code, json);
                break;
            
            default:
                Console.Error.WriteLine($"WARNING: Unknown packet type {packetType}");
                return null;
        }
        
        if (result == null)
        {
            Console.Error.WriteLine($"WARNING: Unknown response code {code}");
            return null;
        }

        VerboseLog($"Successfully parsed as: {result}");
        return result;
    }
    
    private static ProtocolResponse? Deserialize<T>(string json) where T : ProtocolResponse
    {
        return JsonConvert.DeserializeObject<T>(json, CodecConstants.JsonSerializerSettings);
    }
    
    
    //TODO: Consider a logging library
    [Conditional("DEBUG")]
    private static void VerboseLog(string message)
    {
        if (!Communicator.Verbose)
            return;
        
        Log(message);
    }
    
    private static void Log(string message)
    {
        Console.WriteLine($"[{nameof(PacketDeserializer)}] {message}");
    }
}