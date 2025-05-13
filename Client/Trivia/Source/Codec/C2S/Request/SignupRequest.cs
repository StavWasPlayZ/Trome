namespace Trivia.Codec.C2S.Request;

public record SignupRequest(
    string Username,
    string Password,
    string Email,
    string Phone,
    string? Address,
    string Birthday
) : ProtocolRequest(RequestCode.Signup);