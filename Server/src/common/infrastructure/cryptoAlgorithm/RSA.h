#pragma once

#include "ICryptoAlgorithm.h"

#include <cryptopp/cryptlib.h>
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/base64.h>
#include <cryptopp/filters.h>
#include <cryptopp/queue.h>

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

    void loadPrivateKeyFromPEM(const std::string &filename);
    void loadPublicKeyFromPEM(const std::string &filename);
    std::string loadPEMFile(const std::string &filename);

    // Declare these as private member functions
    size_t getMaxPlaintextSize() const;
    size_t getCiphertextSize() const;
};