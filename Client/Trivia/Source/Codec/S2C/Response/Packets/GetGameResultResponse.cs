using System.Collections.Immutable;
using Trivia.Codec.S2C.Objects;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct GetGameResultResponse(
    ImmutableList<PlayerResult> Results
) : IProtocolResponse;
