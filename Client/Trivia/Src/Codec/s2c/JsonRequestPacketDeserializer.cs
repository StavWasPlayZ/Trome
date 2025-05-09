using Newtonsoft.Json;
using Trivia.Codec.s2c.Response;

namespace Trivia.Codec.s2c;

public class JsonRequestPacketDeserializer
{
    /// <summary>
    /// T - Protocol Response
    /// </summary>
    public static T Deserialize<T>(string data)
    {
        return JsonConvert.DeserializeObject<T>(data);
    }
}