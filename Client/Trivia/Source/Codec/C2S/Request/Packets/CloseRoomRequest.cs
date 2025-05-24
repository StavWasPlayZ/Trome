namespace Trivia.Codec.C2S.Request.Packets;

public record CloseRoomRequest() : ProtocolRequest(RequestCode.CloseRoom);