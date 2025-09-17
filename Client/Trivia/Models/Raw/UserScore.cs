namespace Trivia.Models.Raw;

public record UserScore
{
    public User User { get; init; }
    public int Points { get; init; }
}
