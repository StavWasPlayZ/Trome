using Trivia.Codec.C2S.Request;
using Trivia.Codec.S2C.Response.Packets.Impl;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct ErrorResponse(
    ErrorStatus Status,
    RequestCode ReqCode,
    string? Context
) : IProtocolResponse;