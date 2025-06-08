using System.Collections.Immutable;
using Trivia.Codec.S2C.Objects;
using Trivia.Codec.S2C.Response.Packets.Impl;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct GetGameResultResponse(
    ImmutableList<PlayerResult> Results
) : IProtocolResponse;
