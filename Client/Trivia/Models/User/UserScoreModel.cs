namespace Trivia.Models.User;

public readonly record struct UserScoreModel
{
    public Raw.User User { get; init; }
    public int Score { get; init; }
}
