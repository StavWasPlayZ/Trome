using Trivia.Codec.S2C.Response.Status;
using Trivia.Models;

namespace Trivia.Codec.S2C.Response;

public record GetPersonalStatisticsResponse(
    GenericStatsStatus Status,
    UserStatistics Stats
) : ProtocolResponse<GenericStatsStatus>(Status);