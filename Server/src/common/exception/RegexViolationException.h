#pragma once

#include <exception>
#include <string>

class RegexViolationException : public std::exception
{
public:
    explicit RegexViolationException(const std::string& field);
    const char* what() const noexcept override;

private:
    const std::string field;
    const std::string message;
};
