using System.Collections.Generic;
using System.Collections.Immutable;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct GetHighScoresResponse(
    ImmutableList<KeyValuePair<string, int>> Stats
) : IProtocolResponse;
