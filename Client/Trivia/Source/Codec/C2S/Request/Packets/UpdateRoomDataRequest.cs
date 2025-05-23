using Trivia.Models.Raw;

namespace Trivia.Codec.C2S.Request.Packets;

public record UpdateRoomDataRequest(
    RoomData Data
) : ProtocolRequest(RequestCode.UpdateRoomData);