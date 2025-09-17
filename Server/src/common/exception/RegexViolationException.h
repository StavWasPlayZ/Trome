#pragma once

#include <exception>
#include <string>

class RegexViolationException : public std::exception
{
public:
    explicit RegexViolationException(const std::string& field);
    const char* what() const noexcept override;

    const std::string field;
private:
    const std::string message;
};
