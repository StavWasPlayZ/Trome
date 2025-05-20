namespace Trivia.Codec.C2S.Request;

public record StartGameRequest() : ProtocolRequest(RequestCode.StartGame);