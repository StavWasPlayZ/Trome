using Trivia.Codec.S2C.Response.Packets.Impl;
using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response.Packets;

public readonly record struct CreateRoomResponse(
    long RoomId,
    RoomData Data
) : IProtocolResponse;