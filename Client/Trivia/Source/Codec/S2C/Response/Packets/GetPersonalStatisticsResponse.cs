using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Response.Packets;

public record GetPersonalStatisticsResponse(
    UserStatistics Stats
) : ProtocolResponse;