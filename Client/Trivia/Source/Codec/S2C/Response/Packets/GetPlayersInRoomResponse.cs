using System.Collections.Immutable;
using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct GetPlayersInRoomResponse(
    ImmutableList<User> Players
) : IProtocolResponse;