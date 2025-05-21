
namespace Trivia.Codec.S2C.Response;

public record LoginResponse(
    int UserId
) : RegistrationResponse(UserId);