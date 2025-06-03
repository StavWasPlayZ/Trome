using System.Collections.Immutable;
using Trivia.Models;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct GetRoomsResponse(
    ImmutableList<RoomModel> Rooms
) : IProtocolResponse;