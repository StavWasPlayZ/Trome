using Trivia.Codec.C2S;
using Trivia.Codec.S2C.Status;

namespace Trivia.Codec.S2C.Response;

public record ErrorResponse(
    ErrorStatus Status,
    RequestCode ReqCode,
    string? Context
) : ProtocolResponse;