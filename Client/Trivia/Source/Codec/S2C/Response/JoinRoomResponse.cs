using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response;

public record JoinRoomResponse(
    Room Room
) : ProtocolResponse;