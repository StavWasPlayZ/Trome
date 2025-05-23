namespace Trivia.Codec.C2S.Request.Packets;

public record LoginRequest(
    string Username,
    string Password
) : ProtocolRequest(RequestCode.Login);