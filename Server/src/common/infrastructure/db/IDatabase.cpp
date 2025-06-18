#include "IDatabase.h"

#include "exception/RegexViolationException.h"

IDatabase::~IDatabase() = default;

float IDatabase::calcAverageAnswerTime(const int totalTime, const int totalAns)
{
    if (totalAns == -1 || totalAns == 0 || totalTime == -1)
    {
        return -1;
    }

    return static_cast<float>(totalTime) / totalAns;
}
