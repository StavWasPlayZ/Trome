#pragma once

#include "ICryptoAlgorithm.h"

#include <cryptopp/rsa.h>

class RSACrypto : public ICryptoAlgorithm
{
public:
    RSACrypto();

    std::string encrypt(const std::string &message) const override;
    std::string decrypt(const std::string &message) const override;

private:
    static const std::string serverPrivateKeyPath;
    static const std::string clientPublicKeyPath;

    static CryptoPP::RSA::PrivateKey serverPrivateKey;
    static CryptoPP::RSA::PublicKey clientPublicKey;
    static bool keysLoaded;

    // SHA1 hash length = 20 bytes
    static constexpr size_t hashLen = 20;

    static void loadPrivateKeyFromPEM(const std::string &filename);
    static void loadPublicKeyFromPEM(const std::string &filename);
    static std::string loadPEMFile(const std::string &filename);

    // Private member functions
    static size_t getMaxPlaintextSize();
    static size_t getEncryptedTextSize();
};