using Trivia.Codec.S2C.Objects;

namespace Trivia.Codec.S2C.Response.Packets;

public record GetGameResultResponse(
    PlayerResult[] Results
) : ProtocolResponse;