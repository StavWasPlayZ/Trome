namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct KickPlayerRequest(
    long UserId
) : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.KickPlayer;
}