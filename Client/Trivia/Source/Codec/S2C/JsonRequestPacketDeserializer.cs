using Newtonsoft.Json;

namespace Trivia.Codec.S2C;

public static class JsonRequestPacketDeserializer
{
    /// <summary>
    /// T - Protocol Response
    /// </summary>
    public static T? Deserialize<T>(string data)
    {
        return JsonConvert.DeserializeObject<T>(data, CodecConstants.JsonSerializerSettings);
    }
}