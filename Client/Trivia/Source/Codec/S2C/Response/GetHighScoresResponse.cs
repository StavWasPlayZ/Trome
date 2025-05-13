using System.Collections.Generic;
using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

public record GetHighScoresResponse(
    GenericStatsStatus Status,
    IReadOnlyList<KeyValuePair<string, int>> Stats
) : ProtocolResponse<GenericStatsStatus>(Status);