using System;
using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Objects;

public readonly record struct PlayerResult(
    User User,
    int CorrectAnswerCount,
    int AverageAnswerTimeSecs,
    int PlaytimeSecs,
    int Points
)
{
    public TimeSpan AverageAnswerTime => TimeSpan.FromSeconds(AverageAnswerTimeSecs);
};
