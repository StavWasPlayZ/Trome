using System.Collections.Generic;
using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response;

public record GetRoomsResponse(
    List<Room> Rooms
) : ProtocolResponse;