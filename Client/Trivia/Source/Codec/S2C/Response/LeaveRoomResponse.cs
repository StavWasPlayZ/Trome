using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

public record LeaveRoomResponse(
    GenericResponseStatus Status
) : ProtocolResponse<GenericResponseStatus>(Status);