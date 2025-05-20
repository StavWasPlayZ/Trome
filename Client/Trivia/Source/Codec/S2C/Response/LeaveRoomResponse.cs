namespace Trivia.Codec.S2C.Response.Status;

public record LeaveRoomResponse(
        GenericResponseStatus Status
    ) : ProtocolResponse<GenericResponseStatus>(Status);