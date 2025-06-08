#include "NoEncyption.h"

NoEncyption::NoEncyption() : ICryptoAlgorithm()
{}

std::string NoEncyption::encrypt(const std::string &message) const override
{
    return message;
}

std::string NoEncyption::decrypt(const std::string &message) const override
{
    return message;
}