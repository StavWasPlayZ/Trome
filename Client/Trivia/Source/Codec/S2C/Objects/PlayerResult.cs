using System;
using Trivia.Models.Raw;

namespace Trivia.Codec.S2C.Objects;

public record PlayerResult
{
    public required User User { get; init; }
    public required int CorrectAnswerCount { get; init; }
    public required int AverageAnswerTimeSecs { get; init; }
    public required int PlaytimeSecs { get; init; }
    public required int Points { get; init; }
    
    public TimeSpan AverageAnswerTime => TimeSpan.FromSeconds(AverageAnswerTimeSecs);
    public TimeSpan Playtime => TimeSpan.FromSeconds(PlaytimeSecs);
};
