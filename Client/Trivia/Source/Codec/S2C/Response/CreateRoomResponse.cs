using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response;

public record CreateRoomResponse(
    int RoomId,
    RoomData Data
) : ProtocolResponse;