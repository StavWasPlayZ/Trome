namespace Trivia.Codec.C2S.Request;

public record GetPlayersInRoomRequest(
    int RoomId
) : ProtocolRequest(RequestCode.GetPlayersInRoom);