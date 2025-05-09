using System.Text.Json.Nodes;
using Trivia.Codec.c2s.Request;
using Trivia.Codec.s2c.Response;

namespace Trivia.Codec.c2s;
using System;
using System.Text;
using System.Text.Json;
using Newtonsoft.Json;

public class JsonRequestPacketSerializer
{
    private const uint SizeCode = 1;
    private const uint SizeJsonLen = 4;
    public static string serializeJsonToProtocol(ResponseCode code, JsonObject data)
    {
        string dataStr = JsonConvert.SerializeObject(data);
        byte codeByte = (byte)code;
        byte[] strBytes = Encoding.UTF8.GetBytes(dataStr);
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