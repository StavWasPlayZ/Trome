namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct GetGameResultRequest() : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.GetGameResult;
}
