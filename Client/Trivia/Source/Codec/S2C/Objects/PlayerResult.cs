namespace Trivia.Codec.S2C.Objects;

public record PlayerResult(
    string Username,
    uint CorrectAnswerCount,
    uint AverageAnswerTime,
    uint Points
);