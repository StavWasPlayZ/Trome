using Trivia.Models.Raw;

namespace Trivia.Models.User;

public record UserScoreModel
{
    public required int Place { get; init; }
    public UserScore? Scores { get; init; }

    public static UserScoreModel FromUserScore(UserScore score, int place)
    {
        return new UserScoreModel
        {
            Place = place,
            Scores = score
        };
    }
}