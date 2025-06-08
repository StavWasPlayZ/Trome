#include "NoEncryption.h"

NoEncryption::NoEncryption() : ICryptoAlgorithm()
{}

std::string NoEncryption::encrypt(const std::string &message) const override
{
    return message;
}

std::string NoEncryption::decrypt(const std::string &message) const override
{
    return message;
}