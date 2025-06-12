#pragma once

#include "ICryptoAlgorithm.h"

#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <cryptopp/queue.h>

class RSA : public ICryptoAlgorithm
{
public:
    RSA();
    std::string encrypt(const std::string &message) const override;
    std::string decrypt(const std::string &message) const override;

private:
    static const std::string privateServerPath;
    static const std::string publicClientPath;

    static std::string privateServerKeyStr;
    static std::string publicClientKeyStr;

    static CryptoPP::RSA::PrivateKey serverPrivateKey;
    static CryptoPP::RSA::PublicKey clientPublicKey;

    static CryptoPP::AutoSeededRandomPool rng;

    static void LoadPublicKey(const std::string &keyStr);
    static void LoadPrivateKey(const std::string &keyStr);
};