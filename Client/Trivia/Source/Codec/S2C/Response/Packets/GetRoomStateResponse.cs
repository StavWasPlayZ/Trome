using System;
using Trivia.Codec.S2C.Response.Packets.Impl;
using Trivia.Models;

namespace Trivia.Codec.S2C.Response.Packets;

[Obsolete("""
    The Notifications system has been set in place to allow for automatic, non-polling updates of any
    room state changes.
    This request/response is therefore useless and should not be used.
""")]
public readonly record struct GetRoomStateResponse(
    RoomModel Room
) : IProtocolResponse;