using Trivia.Codec.S2C.Response.Status;
using Trivia.Models;

namespace Trivia.Codec.S2C.Response;

public record GetRoomStateResponse(
    GenericResponseStatus Status,
    Room Room
) : ProtocolResponse<GenericResponseStatus>(Status);