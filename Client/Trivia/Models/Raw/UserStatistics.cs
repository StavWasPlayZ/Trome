namespace Trivia.Models.Raw;

public readonly record struct UserStatistics(
    int Points,
    int GamesPlayed,
    int QuestionsAnswered,
    int QuestionsAnsweredCorrectly,
    int TimeOnQuestionOverall,
    int TimeOnQuestionsAvg
);
