using Trivia.Models;

namespace Trivia.Codec.S2C.Response;

public record JoinRoomResponse(
    Room Room
) : ProtocolResponse;