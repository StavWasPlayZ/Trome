namespace Trivia.Codec.C2S.Request.Packets;

public record LeaveGameRequest() : ProtocolRequest(RequestCode.LeaveGame);