namespace Trivia.Codec.C2S.Request.Packets;

public record SignupRequest(
    string Username,
    string Password,
    string Email,
    string Phone,
    string? Address,
    string Birthdate
) : ProtocolRequest(RequestCode.Signup);