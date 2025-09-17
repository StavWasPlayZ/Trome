using Trivia.Codec.S2C.Objects;

namespace Trivia.Models;

public record PlayerResultModel : PlayerResult
{
    public required int Place { get; init; }


    public static PlayerResultModel FromPlayerResult(PlayerResult playerResult, int place)
    {
        return new PlayerResultModel
        {
            Place = place,
            
            User = playerResult.User,
            CorrectAnswerCount = playerResult.CorrectAnswerCount,
            AverageAnswerTimeSecs = playerResult.AverageAnswerTimeSecs,
            PlaytimeSecs = playerResult.PlaytimeSecs,
            Points = playerResult.Points,
        };
    }
};
