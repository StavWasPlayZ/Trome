using System.Text.Json.Nodes;
using Trivia.Codec.c2s.Request;

namespace Trivia.Codec.c2s;
using System;
using System.Text;
using System.Text.Json;
using Newtonsoft.Json;

public class JsonRequestPacketSerializer
{
    private const uint SizeCode = 1;
    private const uint SizeJsonLen = 4;

    public string serializeRequest(LoginRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public string serializeRequest(SignupRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public string serializeRequest(GetPlayersInRoomRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public string serializeRequest(JoinRoomRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public string serializeRequest(GetRoomsRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public string serializeRequest(CreateRoomRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public string serializeRequest(GetHighScoresRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public string serializeRequest(GetPersonalStatisticsRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public static string serializeJsonToProtocol(RequestCode code, string data)
    {
        byte codeByte = (byte)code;
        byte[] strBytes = Encoding.UTF8.GetBytes(data);
        byte[] lenBytes = BitConverter.GetBytes(strBytes.Length);

        if (!BitConverter.IsLittleEndian)
            Array.Reverse(lenBytes); // Makes sure that little-endian if needed

        byte[] result = new byte[1 + 4 + strBytes.Length];
        result[0] = codeByte;
        Array.Copy(lenBytes, 0, result, SizeCode, SizeJsonLen);
        Array.Copy(strBytes, 0, result, SizeCode + SizeJsonLen, strBytes.Length);

        return Encoding.Latin1.GetString(result);
    }
}