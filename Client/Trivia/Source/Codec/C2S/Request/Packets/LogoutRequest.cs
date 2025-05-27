namespace Trivia.Codec.C2S.Request.Packets;

public record LogoutRequest() : ProtocolRequest(RequestCode.Logout);
