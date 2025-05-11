using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;

namespace Trivia.Codec.S2C;

public static class JsonRequestPacketDeserializer
{
    /// <summary>
    /// T - Protocol Response
    /// </summary>
    public static T? Deserialize<T>(string data)
    {
        return JsonConvert.DeserializeObject<T>(data, new JsonSerializerSettings
        {
            // Make the naming strategy be sneak_case and not camelCase.
            // Aligns with server conventions
            ContractResolver = new DefaultContractResolver
            {
                NamingStrategy = new SnakeCaseNamingStrategy()
            }
        });
    }
}