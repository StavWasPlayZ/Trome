#pragma once

#include "ICryptoAlgorithm.h"

#include <cryptopp/rsa.h>

class RSACrypto : public ICryptoAlgorithm
{
public:
    RSACrypto();

    std::string encrypt(const std::string &message) const;
    std::string decrypt(const std::string &base64Ciphertext) const;

private:
    static const std::string serverPrivateKeyPath;
    static const std::string clientPublicKeyPath;

    static CryptoPP::RSA::PrivateKey serverPrivateKey;
    static CryptoPP::RSA::PublicKey clientPublicKey;
    static bool keysLoaded;

    // SHA1 hash length = 20 bytes
    const size_t hashLen = 20;

    void loadPrivateKeyFromPEM(const std::string &filename);
    void loadPublicKeyFromPEM(const std::string &filename);
    std::string loadPEMFile(const std::string &filename);

    // Declare these as private member functions
    size_t getMaxPlaintextSize() const;
    size_t getEncryptedTextSize() const;
};