namespace Trivia.Codec.C2S.Request.Packets;

public record CloseGameRequest() : ProtocolRequest(RequestCode.CloseGame);