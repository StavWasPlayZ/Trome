using Trivia.Codec.C2S.Request;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct ErrorResponse(
    ErrorStatus Status,
    RequestCode ReqCode,
    string? Context
) : IProtocolResponse;