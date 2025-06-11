using System.Collections.Immutable;
using Trivia.Codec.S2C.Response.Packets.Impl;
using Trivia.Models;
using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct JoinRoomResponse(
    RoomModel Room,
    ImmutableList<User> Players
) : IProtocolResponse;