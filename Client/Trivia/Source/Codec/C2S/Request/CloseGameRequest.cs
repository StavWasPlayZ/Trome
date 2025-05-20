namespace Trivia.Codec.C2S.Request;

public record CloseGameRequest() : ProtocolRequest(RequestCode.CloseGame);