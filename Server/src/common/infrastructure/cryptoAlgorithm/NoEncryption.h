#pragma once

#include "ICryptoAlgorithm.h"

class NoEncryption : public ICryptoAlgorithm
{
public:
    NoEncryption();
    std::string encrypt(const std::string &message) const override;
    std::string decrypt(const std::string &message) const override;
};