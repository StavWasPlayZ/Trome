using System;
using System.Text;
using Newtonsoft.Json;

namespace Trivia.Codec.C2S.Request;

public abstract record ProtocolRequest(
    RequestCode Code
)
{
    public byte[] Serialize()
    {
        return SerializeToProtocol(
            JsonConvert.SerializeObject(this, CodecConstants.JsonSerializerSettings)
        );
    }
    
    private byte[] SerializeToProtocol(string data)
    {
        var strBytes = Encoding.UTF8.GetBytes(data);
        var lenBytes = BitConverter.GetBytes(strBytes.Length);

        // Convert to little-endian format, if needed
        if (BitConverter.IsLittleEndian)
        {
            Array.Reverse(lenBytes);
        }

        var result = new byte[CodecConstants.SizeCode + CodecConstants.SizeJsonLen + strBytes.Length];
        
        result[0] = (byte) Code;
        
        Array.Copy(
            lenBytes,
            0,
            result,
            CodecConstants.SizeCode,
            CodecConstants.SizeJsonLen
        );
        Array.Copy(
            strBytes,
            0,
            result,
            CodecConstants.SizeCode + CodecConstants.SizeJsonLen, strBytes.Length
        );

        return result;
    }
}
