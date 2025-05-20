using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

public record StartRoomResponse(
    GenericResponseStatus Status
) : ProtocolResponse<GenericResponseStatus>(Status);