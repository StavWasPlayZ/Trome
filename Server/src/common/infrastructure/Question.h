#pragma once

#include <string>
#include <vector>

struct Question {
    Question(const std::string &question, const std::vector<std::string> &answers);

    const std::string question;

    /**
     * All the answers of this question. 4 total.
     * The first one is always the correct one.
     */
    const std::vector<std::string> answers;

    /**
     * Returns a new answers list where the answers are rotated by the provided amount.
     *
     * The rotation may be 0 to 3.
     */
    std::vector<std::string> getAnswersRotated(int rotation) const;
};
