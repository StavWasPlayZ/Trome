using Newtonsoft.Json;
using Newtonsoft.Json.Serialization;

namespace Trivia.Codec;

public static class CodecConstants
{
    public const int SizeCode = sizeof(byte);
    public const int SizeJsonLen = sizeof(int);
    
    public static readonly JsonSerializerSettings JsonSerializerSettings = new()
    {
        // Make the naming strategy be sneak_case and not camelCase.
        // Aligns with server conventions.
        ContractResolver = new DefaultContractResolver
        {
            NamingStrategy = new SnakeCaseNamingStrategy()
        }
    };
}