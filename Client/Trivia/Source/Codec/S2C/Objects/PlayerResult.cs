using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Objects;

public readonly record struct PlayerResult(
    User User,
    int CorrectAnswerCount,
    int AverageAnswerTimeSecs,
    int Points
);
