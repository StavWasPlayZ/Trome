namespace Trivia.Codec;

public class UserStatistics
{
    public UserStatistics(int points, int gamesPlayed, int questionsAnswered, int questionsAnsweredCorrectly,
        int timeOnQuestionOverall, int timeOnQuestionsAvg)
    {
        this.Points = points;
        this.GamesPlayed = gamesPlayed;
        this.QuestionsAnswered = questionsAnswered;
        this.QuestionsAnsweredCorrectly = questionsAnsweredCorrectly;
        this.TimeOnQuestionOverall = timeOnQuestionOverall;
        this.TimeOnQuestionAvg = timeOnQuestionsAvg;
    }
    
    public readonly int Points;
    public readonly int GamesPlayed;
    public readonly int QuestionsAnswered;
    public readonly int QuestionsAnsweredCorrectly;
    public readonly int TimeOnQuestionOverall;
    public readonly int TimeOnQuestionAvg;
}