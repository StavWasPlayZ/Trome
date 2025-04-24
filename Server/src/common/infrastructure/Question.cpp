#include "Question.h"

#include <algorithm>
#include <stdexcept>

Question::Question(const std::string &question, const std::vector<std::string> &answers)
    : question(question), answers(answers)
{}

std::vector<std::string> Question::getAnswersRotated(const int rotation) const
{
    if (rotation == 0)
        return answers;

    if (rotation >= QUESTIONS_AMOUNT || rotation < 0)
    {
        throw std::invalid_argument("Invalid rotation");
    }

    std::vector<std::string> result = this->answers;

    std::ranges::rotate(result, result.begin() + rotation);

    return result;
}
