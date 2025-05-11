using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

public record LoginResponse(
    LoginStatus Status,
    int? UserId
) : RegistrationResponse<LoginStatus>(Status, UserId);