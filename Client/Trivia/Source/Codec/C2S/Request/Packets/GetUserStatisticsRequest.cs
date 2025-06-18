namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct GetUserStatisticsRequest(
    long UserId
) : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.GetUserStatistics;
}
