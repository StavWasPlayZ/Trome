namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct LeaveGameRequest() : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.LeaveGame;
}