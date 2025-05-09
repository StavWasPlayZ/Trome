using System.Text.Json.Nodes;

namespace Trivia.Codec.c2s;
using System;
using System.Text;
using System.Text.Json;
using Newtonsoft.Json;

public class JsonRequestPacketDeserializer
{
    public static JsonObject ReadJson(byte[] data, int jsonLen)
    {
	    if (jsonLen <= 0 || data == null || data.Length < jsonLen)
	    {
		    throw new ArgumentException("Invalid JSON length or data buffer");
	    }
	    
	    string jsonString = Encoding.UTF8.GetString(data, 0, jsonLen);
	    JsonNode? node = JsonNode.Parse(jsonString);
	    
	    if (node is JsonObject obj)
	    {
		    return obj;
	    }

	    throw new InvalidOperationException("Parsed JSON is not an object");
    }
    
    public uint toUL(int n)
    {
	    if (n < 0)
	    {
		    throw new ArgumentException("Invalid room ID");
	    }
	    
	    return (uint)n;
    }
}