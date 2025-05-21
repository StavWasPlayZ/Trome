using System.Collections.Generic;

namespace Trivia.Codec.S2C.Response;

public record GetHighScoresResponse(
    IReadOnlyList<KeyValuePair<string, int>> Stats
) : ProtocolResponse;
