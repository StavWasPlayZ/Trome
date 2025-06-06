using Trivia.Models.Raw;

namespace Trivia.Models.User;

public record UserScoreModel : UserScore
{
    public required int Place { get; init; }


    public static UserScoreModel FromUserScore(UserScore score, int place)
    {
        return new UserScoreModel
        {
            User = score.User,
            Place = place,
            Points = score.Points
        };
    }
}