using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

//TODO: When Room is implemented, add below.
public record GetPlayersInRoomResponse(
    GenericResponseStatus Status
) : ProtocolResponse<GenericResponseStatus>(Status);