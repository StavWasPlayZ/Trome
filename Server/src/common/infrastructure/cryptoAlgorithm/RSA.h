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

	std::string encrypt(const std::string &message) const override;
	std::string decrypt(const std::string &message) const override;

private:
    static const std::string serverPrivateKeyPath;
	static const std::string clientPublicKeyPath;
	static bool keysLoaded;
	static CryptoPP::RSA::PrivateKey serverPrivateKey;
	static CryptoPP::RSA::PublicKey clientPublicKey;

  
	static std::string loadPEMFile(const std::string &filename);
	static void loadPrivateKeyFromPEM(const std::string &filename);
	static void loadPublicKeyFromPEM(const std::string &filename);
};
