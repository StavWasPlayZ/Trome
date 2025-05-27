namespace Trivia.Codec.C2S.Request.Packets;

public record GetPersonalStatisticsRequest()
    : ProtocolRequest(RequestCode.GetPersonalStatistics);