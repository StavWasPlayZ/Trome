namespace Trivia.Codec.C2S.Request.Packets;

public record GetPlayersInRoomRequest(
    int RoomId
) : ProtocolRequest(RequestCode.GetPlayersInRoom);