using System;
using System.Text;
using Newtonsoft.Json;
using Trivia.CryptoAlgorithm;

namespace Trivia.Codec.C2S.Request;

public interface IProtocolRequest
{
    RequestCode Code { get; init; }
    
    public byte[] Serialize(ICryptoAlgorithm cryptoAlgorithm)
    {
        return SerializeToProtocol(
            JsonConvert.SerializeObject(this, CodecConstants.JsonSerializerSettings),
            cryptoAlgorithm
        );
    }
    
    private byte[] SerializeToProtocol(string data, ICryptoAlgorithm cryptoAlgorithm)
    {
        var strBytes = Encoding.GetEncoding("ISO-8859-1").GetBytes(cryptoAlgorithm.Encrypt(data));
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
