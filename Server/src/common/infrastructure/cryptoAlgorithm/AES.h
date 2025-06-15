#pragma once

#include "ICryptoAlgorithm.h"
#include <vector>

class AES : public ICryptoAlgorithm
{
public:
    AES();
    std::string encrypt(const std::string &message) const override;
    std::string decrypt(const std::string &message) const override;

private:
    static const std::string keyPath;
    static const std::string ivPath;
    static std::vector<unsigned char> key;
    static std::vector<unsigned char> iv;
};