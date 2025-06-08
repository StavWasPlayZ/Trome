#pragma once

#include "ICryptoAlgorithm.h"

class NoEncyption : public ICryptoAlgorithm
{
public:
    NoEncyption();
    std::string encrypt(const std::string &message) const override;
    std::string decrypt(const std::string &message) const override;
};