using Trivia.Codec.S2C.Response.Status;
using Trivia.Models;

namespace Trivia.Codec.S2C.Response;

public record JoinRoomResponse(
    ConsumingResponseStatus Status,
    RoomStatus RoomStatus,
    RoomData Data
) : ProtocolResponse<ConsumingResponseStatus>(Status);