using System;

namespace Trivia.Codec.C2S.Request.Packets;

[Obsolete("""
    The Notifications system has been set in place to allow for automatic, non-polling updates of any
    room state changes.
    This request/response is therefore useless and should not be used.
""")]
public readonly record struct GetRoomStateRequest() : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.GetRoomState;
}
