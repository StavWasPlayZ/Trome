namespace Trivia.Codec.C2S.Request.Packets;

public record CreateRoomRequest() : ProtocolRequest(RequestCode.CreateRoom);