#pragma once

struct UserStatistics
{
    UserStatistics(int points, int gamesPlayed, int questionsAnswered, int questionsAnsweredCorrect,
                   int timeOnQuestionsOverall, int timePerQuestionsAvg);

    const int points;
    const int gamesPlayed;
    const int questionsAnswered;
    const int questionsAnsweredCorrect;
    const int timeOnQuestionsOverall;
    const int timePerQuestionsAvg;
};
