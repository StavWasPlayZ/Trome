namespace Trivia.Codec.C2S.Request;

public record GetPersonalStatisticsRequest()
    : ProtocolRequest(RequestCode.GetPersonalStatistics);