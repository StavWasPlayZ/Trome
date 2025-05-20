namespace Trivia.Codec.C2S.Request;

public record LeaveRoomRequest() : ProtocolRequest(RequestCode.LeaveRoom);