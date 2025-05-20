using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

public record UpdateRoomDataResponse(
        GenericResponseStatus Status
    ) : ProtocolResponse<GenericResponseStatus>(Status);