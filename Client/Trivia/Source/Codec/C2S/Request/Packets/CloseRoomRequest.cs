namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct CloseRoomRequest() : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.CloseRoom;
}