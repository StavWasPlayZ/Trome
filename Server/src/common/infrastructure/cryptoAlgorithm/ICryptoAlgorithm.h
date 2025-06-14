#pragma once

#include <string>

class ICryptoAlgorithm
{
public:
    ICryptoAlgorithm();
    virtual ~ICryptoAlgorithm();

    virtual std::string encrypt(const std::string &message) const = 0;
    virtual std::string decrypt(const std::string &message) const = 0;
};