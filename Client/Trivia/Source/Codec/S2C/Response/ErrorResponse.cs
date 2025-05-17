using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

public record ErrorResponse(
    ErrorStatus Status,
    string Message
) : ProtocolResponse<ErrorStatus>(Status);