namespace Trivia.Codec.C2S.Request.Packets;

public record GetRoomStateRequest() : ProtocolRequest(RequestCode.GetRoomState);