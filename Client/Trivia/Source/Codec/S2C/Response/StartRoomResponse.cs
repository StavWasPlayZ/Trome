namespace Trivia.Codec.S2C.Response.Status;

public record StartRoomResponse(
        GenericResponseStatus Status
    ) : ProtocolResponse<GenericResponseStatus>(Status);