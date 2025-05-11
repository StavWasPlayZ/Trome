using System;
using System.Text;
using Newtonsoft.Json;
using Trivia.Codec.C2S.Request;

namespace Trivia.Codec.C2S;

public class JsonRequestPacketSerializer
{
    private const uint SizeCode = 1;
    private const uint SizeJsonLen = 4;

    public byte[] SerializeRequest(LoginRequest request)
    {
        return SerializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] SerializeRequest(SignupRequest request)
    {
        return SerializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] SerializeRequest(GetPlayersInRoomRequest request)
    {
        return SerializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] SerializeRequest(JoinRoomRequest request)
    {
        return SerializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] SerializeRequest(GetRoomsRequest request)
    {
        return SerializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] SerializeRequest(CreateRoomRequest request)
    {
        return SerializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] SerializeRequest(GetHighScoresRequest request)
    {
        return SerializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public byte[] SerializeRequest(GetPersonalStatisticsRequest request)
    {
        return SerializeJsonToProtocol(RequestCode.Login, JsonConvert.SerializeObject(request));
    }
    public static byte[] SerializeJsonToProtocol(RequestCode code, string data)
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