namespace Trivia.Codec.S2C.Objects;

public record QuestionObject(
    string Question,
    string[] Answers
);