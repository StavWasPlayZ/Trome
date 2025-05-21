using System.Collections.Generic;
using Trivia.Codec.S2C.Response.Status;
using Trivia.Models;

namespace Trivia.Codec.S2C.Response;

public record GetPlayersInRoomResponse(
    GenericResponseStatus Status,
    List<User> Players
) : ProtocolResponse<GenericResponseStatus>(Status);