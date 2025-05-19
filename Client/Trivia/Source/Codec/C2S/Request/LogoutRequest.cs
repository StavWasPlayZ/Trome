namespace Trivia.Codec.C2S.Request;

public record LogoutRequest() : ProtocolRequest(RequestCode.Logout);
