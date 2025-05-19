using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

public record SignupResponse(
    SignupStatus Status,
    string? Context,
    int? UserId = null
) : RegistrationResponse<SignupStatus>(Status, UserId);