using System.Text.Json.Nodes;
using Trivia.Codec.c2s.Request;

namespace Trivia.Codec.c2s;
using System;
using System.Text;
using System.Text.Json;
using Newtonsoft.Json;

public class JsonRequestPacketDeserializer
{
    public static LoginRequest deserializeLoginRequest(string data)
    {
	    return JsonConvert.DeserializeObject<LoginRequest>(data);
    }
}