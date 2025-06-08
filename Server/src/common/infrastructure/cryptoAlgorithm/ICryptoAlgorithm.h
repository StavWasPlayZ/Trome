#pragma once

#include <string>

class ICryptoAlgorithm
{
public:
    explicit ICryptoAlgorithm();
    virtual std::string encrypt(const std::string &message) = 0;
    virtual std::string decrypt(const std::string &message) = 0;
};