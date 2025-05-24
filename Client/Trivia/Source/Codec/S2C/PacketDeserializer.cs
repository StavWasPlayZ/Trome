using System;
using System.Diagnostics;
using Newtonsoft.Json;
using Trivia.Codec.S2C.Notification;
using Trivia.Codec.S2C.Response;

namespace Trivia.Codec.S2C;

public static class PacketDeserializer
{   
    public static IS2CPacket? Deserialize(S2CPacketType packetType, byte code, string json)
    {
        IS2CPacket? result;

        switch (packetType)
        {
            case S2CPacketType.Response:
                VerboseLog($"Received response of code {code}: {json}");
                result = ResponseDeserializer.Deserialize((ResponseCode) code, json);
                break;
            
            case S2CPacketType.Notification:
                VerboseLog($"Received notification of code {code}: {json}");
                result = NotificationDeserializer.Deserialize((NotificationCode) code, json);
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
    
    public static T? Deserialize<T>(string json)
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