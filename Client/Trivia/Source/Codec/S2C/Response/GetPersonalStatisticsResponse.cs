using Trivia.Models.User;

namespace Trivia.Codec.S2C.Response;

public record GetPersonalStatisticsResponse(
    UserStatistics Stats
) : ProtocolResponse;