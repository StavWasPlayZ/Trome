namespace Trivia.Codec.S2C.Objects;

public record PlayerResult(
    string Username,
    int CorrectAnswerCount,
    int AverageAnswerTime,
    int Points
);