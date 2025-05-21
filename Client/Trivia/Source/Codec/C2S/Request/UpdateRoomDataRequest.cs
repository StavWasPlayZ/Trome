using Trivia.Models;

namespace Trivia.Codec.C2S.Request;

public record UpdateRoomDataRequest(
    RoomMetadata Metadata
) : ProtocolRequest(RequestCode.UpdateRoomData);