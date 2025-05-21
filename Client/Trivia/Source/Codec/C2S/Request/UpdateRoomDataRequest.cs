using Trivia.Models;

namespace Trivia.Codec.C2S.Request;

public record UpdateRoomDataRequest(
    RoomData Data
) : ProtocolRequest(RequestCode.UpdateRoomData);