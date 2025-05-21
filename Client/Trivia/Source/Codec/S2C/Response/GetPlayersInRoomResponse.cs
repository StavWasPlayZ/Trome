using System.Collections.Generic;
using Trivia.Models;

namespace Trivia.Codec.S2C.Response;

public record GetPlayersInRoomResponse(
    List<User> Players
) : ProtocolResponse;