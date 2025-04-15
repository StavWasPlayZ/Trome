#include "IDatabase.h"

#include "exception/RegexViolationException.h"

IDatabase::~IDatabase() = default;

void IDatabase::validateSignupInfo(
    const std::string& password,
    const std::string& email,
    const std::string& phone,
    const std::string& birthdate,
    const std::optional<std::string>& address)
{
    if (!passMatcher.match(password))
        throw RegexViolationException("password");

    if (!emailMatcher.match(email))
        throw RegexViolationException("email");

    if (!phoneMatcher.match(phone))
        throw RegexViolationException("phone");

    if (!dateMatcher.match(birthdate))
        throw RegexViolationException("birthdate");

    if (address.has_value() && !addressMatcher.match(address.value()))
        throw RegexViolationException("address");
}
