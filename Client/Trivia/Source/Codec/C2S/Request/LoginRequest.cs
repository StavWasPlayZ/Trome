namespace Trivia.Codec.C2S.Request;

public record LoginRequest(
    string Username,
    string Password
) : ProtocolRequest(RequestCode.Login);