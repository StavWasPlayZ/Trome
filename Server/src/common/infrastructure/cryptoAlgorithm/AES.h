#pragma once

#include "ICryptoAlgorithm.h"
#include <vector>

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cryptopp/secblock.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/osrng.h>

class AES : public ICryptoAlgorithm
{
public:
    AES();
    std::string encrypt(const std::string &message) const override;
    std::string decrypt(const std::string &message) const override;

private:
    static const std::vector<unsigned char> key;
    static const std::vector<unsigned char> iv;
};