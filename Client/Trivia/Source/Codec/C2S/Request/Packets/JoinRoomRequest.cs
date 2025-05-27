namespace Trivia.Codec.C2S.Request.Packets;

public record JoinRoomRequest(
    int RoomId
) : ProtocolRequest(RequestCode.JoinRoom);