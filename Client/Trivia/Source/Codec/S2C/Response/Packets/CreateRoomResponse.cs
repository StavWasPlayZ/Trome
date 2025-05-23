using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response.Packets;

public record CreateRoomResponse(
    int RoomId,
    RoomData Data
) : ProtocolResponse;