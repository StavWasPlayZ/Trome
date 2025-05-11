using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;

namespace Trivia.Codec;

public static class CodecConstants
{
    public static readonly JsonSerializerSettings JsonSerializerSettings = new()
    {
        // Make the naming strategy be sneak_case and not camelCase.
        // Aligns with server conventions
        ContractResolver = new DefaultContractResolver
        {
            NamingStrategy = new SnakeCaseNamingStrategy()
        }
    };
}