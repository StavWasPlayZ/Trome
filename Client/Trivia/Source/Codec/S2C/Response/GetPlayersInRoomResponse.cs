using System.Collections.Generic;
using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response;

public record GetPlayersInRoomResponse(
    List<User> Players
) : ProtocolResponse;