#include "UserStatistics.h"

UserStatistics::UserStatistics(const int points, const int gamesPlayed, const int questionsAnswered,
                               const int questionsAnsweredCorrect, const int timeOnQuestionsOverall,
                               const int timePerQuestionsAvg)
    : points(points), gamesPlayed(gamesPlayed), questionsAnswered(questionsAnswered),
      questionsAnsweredCorrect(questionsAnsweredCorrect), timeOnQuestionsOverall(timeOnQuestionsOverall),
      timePerQuestionsAvg(timePerQuestionsAvg)
{}
