namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct GetRoomStateRequest() : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.GetRoomState;
}