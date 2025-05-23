
namespace Trivia.Codec.S2C.Response.Packets;

public record LoginResponse(
    int UserId
) : RegistrationResponse(UserId);