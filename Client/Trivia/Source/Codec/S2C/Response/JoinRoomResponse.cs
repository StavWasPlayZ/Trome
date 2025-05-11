using Trivia.Codec.S2C.Response.Status;

namespace Trivia.Codec.S2C.Response;

public record JoinRoomResponse(
    ConsumingResponseStatus Status
) : ProtocolResponse<ConsumingResponseStatus>(Status);