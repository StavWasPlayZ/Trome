using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

//TODO: Add rooms metadata
public record CreateRoomResponse(
    GenericResponseStatus Status,
    int RoomId
) : ProtocolResponse<GenericResponseStatus>(Status);