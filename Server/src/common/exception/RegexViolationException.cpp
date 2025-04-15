#include "RegexViolationException.h"

RegexViolationException::RegexViolationException(const std::string &field) :
    field(field),
    message(field + " was an invalid format")
{}

const char* RegexViolationException::what() const noexcept
{
    return message.c_str();
}
