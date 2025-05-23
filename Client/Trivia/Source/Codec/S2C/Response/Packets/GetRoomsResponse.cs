using System.Collections.Generic;
using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response.Packets;

public record GetRoomsResponse(
    List<Room> Rooms
) : ProtocolResponse;