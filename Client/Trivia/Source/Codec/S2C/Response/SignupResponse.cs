
namespace Trivia.Codec.S2C.Response;

public record SignupResponse(
    int UserId
) : RegistrationResponse(UserId);