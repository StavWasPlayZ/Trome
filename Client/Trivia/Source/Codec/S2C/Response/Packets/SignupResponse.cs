
namespace Trivia.Codec.S2C.Response.Packets;

public record SignupResponse(
    int UserId
) : RegistrationResponse(UserId);