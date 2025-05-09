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

    public byte[] serializeRequest(LoginRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] serializeRequest(SignupRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] serializeRequest(GetPlayersInRoomRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] serializeRequest(JoinRoomRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] serializeRequest(GetRoomsRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] serializeRequest(CreateRoomRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] serializeRequest(GetHighScoresRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] serializeRequest(GetPersonalStatisticsRequest request)
    {
        return serializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public static byte[] serializeJsonToProtocol(RequestCode code, string data)
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

        return result;
    }
}