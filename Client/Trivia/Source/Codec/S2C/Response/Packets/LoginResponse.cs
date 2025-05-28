
namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct LoginResponse(
    int UserId
) : IRegistrationResponse;
