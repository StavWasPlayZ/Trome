using Trivia.Codec.S2C.Response.Status;
using Trivia.Models;

namespace Trivia.Codec.S2C.Response;

public record GetRoomStateResponse(
    GenericResponseStatus Status,
    RoomStatus GameStatus,
    RoomMetadata Metadata
) : ProtocolResponse<GenericResponseStatus>(Status);