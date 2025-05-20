namespace Trivia.Codec.C2S.Request;

public record GetRoomStateRequest() : ProtocolRequest(RequestCode.GetRoomState);