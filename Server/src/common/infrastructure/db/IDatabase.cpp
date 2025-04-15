#include "IDatabase.h"

#include "exception/RegexViolationException.h"

IDatabase::~IDatabase() = default;

void IDatabase::validateSignupInfo(
    const std::string& email,
    const std::string& phone,
    const std::string& birthdate,
    const std::optional<std::string>& address
) {
    if (!emailMatcher.match(email))
        throw RegexViolationException("email");

    if (!phoneMatcher.match(phone))
        throw RegexViolationException("phone");

    if (!emailMatcher.match(birthdate)) // fix this
        throw RegexViolationException("birthdate");

    if (address.has_value() && !emailMatcher.match(address.value()))
        throw RegexViolationException("address");
}
