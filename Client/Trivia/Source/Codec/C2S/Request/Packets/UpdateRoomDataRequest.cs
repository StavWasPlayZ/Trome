using Trivia.Models.Raw;

namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct UpdateRoomDataRequest(
    RoomData Data
) : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.UpdateRoomData;
}
