namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct LeaveRoomRequest() : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.LeaveRoom;
}