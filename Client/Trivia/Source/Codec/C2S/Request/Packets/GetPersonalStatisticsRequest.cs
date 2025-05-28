namespace Trivia.Codec.C2S.Request.Packets;

public readonly record struct GetPersonalStatisticsRequest() : IProtocolRequest
{
    public RequestCode Code { get; init; } = RequestCode.GetPersonalStatistics;
}