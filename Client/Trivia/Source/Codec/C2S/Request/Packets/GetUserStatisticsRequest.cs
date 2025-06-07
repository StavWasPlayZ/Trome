namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct GetUserStatisticsRequest(
    int UserId
) : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.GetUserStatistics;
}
