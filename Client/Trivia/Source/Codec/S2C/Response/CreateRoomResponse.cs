using Trivia.Codec.S2C.Response.Status;
using Trivia.Models;

namespace Trivia.Codec.S2C.Response;

public record CreateRoomResponse(
    GenericResponseStatus Status,
    int RoomId,
    RoomData Data
) : ProtocolResponse<GenericResponseStatus>(Status);