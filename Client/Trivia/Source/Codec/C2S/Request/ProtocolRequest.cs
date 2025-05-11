using System;
using System.Text;
using Newtonsoft.Json;

namespace Trivia.Codec.C2S.Request;

public abstract record ProtocolRequest(
    RequestCode Code
)
{
    private const uint SizeCode = 1;
    private const uint SizeJsonLen = 4;
    
    public byte[] SerializeRequest()
    {
        return SerializeJsonToProtocol(
            JsonConvert.SerializeObject(this, CodecConstants.JsonSerializerSettings)
        );
    }
    
    private byte[] SerializeJsonToProtocol(string data)
    {
        var strBytes = Encoding.UTF8.GetBytes(data);
        var lenBytes = BitConverter.GetBytes(strBytes.Length);

        if (!BitConverter.IsLittleEndian)
        {
            Array.Reverse(lenBytes); // Makes sure that little-endian, if needed
        }

        var result = new byte[SizeCode + SizeJsonLen + strBytes.Length];
        result[0] = (byte) Code;
        Array.Copy(lenBytes, 0, result, SizeCode, SizeJsonLen);
        Array.Copy(strBytes, 0, result, SizeCode + SizeJsonLen, strBytes.Length);

        return result;
    }
}
