namespace Trivia.Models.User;

public record UserStatistics(
    int Points,
    int GamesPlayed,
    int QuestionsAnswered,
    int QuestionsAnsweredCorrectly,
    int TimeOnQuestionOverall,
    int TimeOnQuestionsAvg
);