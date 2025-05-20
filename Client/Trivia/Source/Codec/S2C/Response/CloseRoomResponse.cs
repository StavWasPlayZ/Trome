using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

public record CloseRoomResponse(
        GenericResponseStatus Status
    ) : ProtocolResponse<GenericResponseStatus>(Status);