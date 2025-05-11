using System;
using System.Text;
using Newtonsoft.Json;
using Trivia.Codec.C2S.Request;

namespace Trivia.Codec.C2S;

public static class JsonRequestPacketSerializer
{
    private const uint SizeCode = 1;
    private const uint SizeJsonLen = 4;

    //TODO: Check if may reduce to just the method below.
    // This may be problematic for per-object serialization.
    public static byte[] SerializeRequest(ProtocolRequest request)
    {
        return SerializeJsonToProtocol(request.Code, JsonConvert.SerializeObject(request));
    }
    
    public static byte[] SerializeRequest(LoginRequest request)
    {
        return SerializeJsonToProtocol(request.Code, JsonConvert.SerializeObject(request));
    }
    public static byte[] SerializeRequest(SignupRequest request)
    {
        return SerializeJsonToProtocol(request.Code, JsonConvert.SerializeObject(request));
    }
    public static byte[] SerializeRequest(GetPlayersInRoomRequest request)
    {
        return SerializeJsonToProtocol(request.Code, JsonConvert.SerializeObject(request));
    }
    public static byte[] SerializeRequest(JoinRoomRequest request)
    {
        return SerializeJsonToProtocol(request.Code, JsonConvert.SerializeObject(request));
    }
    public static byte[] SerializeRequest(GetRoomsRequest request)
    {
        return SerializeJsonToProtocol(request.Code, JsonConvert.SerializeObject(request));
    }
    public static byte[] SerializeRequest(CreateRoomRequest request)
    {
        return SerializeJsonToProtocol(request.Code, JsonConvert.SerializeObject(request));
    }
    public static byte[] SerializeRequest(GetHighScoresRequest request)
    {
        return SerializeJsonToProtocol(request.Code, JsonConvert.SerializeObject(request));
    }
    public static byte[] SerializeRequest(GetPersonalStatisticsRequest request)
    {
        return SerializeJsonToProtocol(request.Code, JsonConvert.SerializeObject(request));
    }

    private static byte[] SerializeJsonToProtocol(RequestCode code, string data)
    {
        var codeByte = (byte)code;
        var strBytes = Encoding.UTF8.GetBytes(data);
        var lenBytes = BitConverter.GetBytes(strBytes.Length);

        if (!BitConverter.IsLittleEndian)
            Array.Reverse(lenBytes); // Makes sure that little-endian if needed

        var result = new byte[1 + 4 + strBytes.Length];
        result[0] = codeByte;
        Array.Copy(lenBytes, 0, result, SizeCode, SizeJsonLen);
        Array.Copy(strBytes, 0, result, SizeCode + SizeJsonLen, strBytes.Length);

        return result;
    }
}