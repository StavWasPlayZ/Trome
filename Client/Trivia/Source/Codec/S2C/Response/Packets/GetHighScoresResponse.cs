using System.Collections.Generic;

namespace Trivia.Codec.S2C.Response.Packets;

public record GetHighScoresResponse(
    List<KeyValuePair<string, int>> Stats
) : ProtocolResponse;
