#pragma once

#include <string>
#include <vector>

#include "Utils.h"

class ICryptoAlgorithm
{
public:
    virtual ~ICryptoAlgorithm();

    virtual std::string encrypt(const std::string &message) const = 0;
    virtual std::string decrypt(const std::string &message) const = 0;

protected:
    static const std::string KEYS_PATH;
};