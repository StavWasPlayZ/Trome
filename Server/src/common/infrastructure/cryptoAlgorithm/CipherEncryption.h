#pragma once

#include "ICryptoAlgorithm.h"

/*
* A test class encryption
* No practical use
*/
class CipherEncryption : public ICryptoAlgorithm
{
public:
    std::string encrypt(const std::string &message) const override;
    std::string decrypt(const std::string &message) const override;

private:
    static constexpr int key = 3;
};