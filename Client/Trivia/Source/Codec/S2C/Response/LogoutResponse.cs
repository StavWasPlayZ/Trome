using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

public record LogoutResponse(
    LogoutStatus Status
) : ProtocolResponse<LogoutStatus>(Status);