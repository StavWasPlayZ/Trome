#pragma once

#include "ICryptoAlgorithm.h"

/*
* A test class
* No practical use
*/
class CipherEncryption : public ICryptoAlgorithm
{
  public:
    CipherEncryption();
    std::string encrypt(const std::string &message) const override;
    std::string decrypt(const std::string &message) const override;

private:
    static const int key = 3;
};