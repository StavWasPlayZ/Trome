namespace Trivia.Models.User;

public class UserStatistics
{
    public required int Points { get; init; }
    public required int GamesPlayed { get; init; }
    public required int QuestionsAnswered { get; init; }
    public required int QuestionsAnsweredCorrectly { get; init; }
    public required int TimeOnQuestionOverall { get; init; }
    public required int TimeOnQuestionsAvg { get; init; }
}