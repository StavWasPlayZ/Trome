namespace Trivia.Codec.C2S.Request;

public record JoinRoomRequest(
    int RoomId
) : ProtocolRequest(RequestCode.JoinRoom);