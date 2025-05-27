namespace Trivia.Codec.C2S.Request.Packets;

public record StartGameRequest() : ProtocolRequest(RequestCode.StartGame);