#include "NoEncyption.h"

std::string NoEncyption::encrypt(const std::string &message) const override
{
    return message;
}

std::string NoEncyption::decrypt(const std::string &message) const override
{
    return message;
}