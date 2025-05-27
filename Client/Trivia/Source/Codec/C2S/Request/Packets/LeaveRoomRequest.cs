namespace Trivia.Codec.C2S.Request.Packets;

public record LeaveRoomRequest() : ProtocolRequest(RequestCode.LeaveRoom);