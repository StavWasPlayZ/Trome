namespace Trivia.Models.Raw;

public record UserStatistics
{
    public required int Points { get; init; }
    public required int GamesPlayed { get; init; }
    public required int QuestionsAnswered { get; init; }
    public required int QuestionsAnsweredCorrectly { get; init; }
    public required int TimeOnQuestionOverall { get; init; }
    public required int TimeOnQuestionsAvg { get; init; }
}
