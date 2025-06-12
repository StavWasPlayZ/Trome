#include "RSA.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>

const std::string RSACrypto::serverPrivateKeyPath = "../../../src/common/infrastructure/cryptoAlgorithm/keys/privateServer.pem";
const std::string RSACrypto::clientPublicKeyPath = "../../../src/common/infrastructure/cryptoAlgorithm/keys/publicClient.pem";
CryptoPP::RSA::PrivateKey RSACrypto::serverPrivateKey;
CryptoPP::RSA::PublicKey RSACrypto::clientPublicKey;

bool RSACrypto::keysLoaded = false;

RSACrypto::RSACrypto()
{
    if (!keysLoaded)
    {
        loadPrivateKeyFromPEM(serverPrivateKeyPath);
        loadPublicKeyFromPEM(clientPublicKeyPath);
        keysLoaded = true;
    }
}

std::string RSACrypto::loadPEMFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw FileNotFoundException(filename);
        
    }

    std::ostringstream oss;
    std::string line;
    bool inKey = false;
    while (std::getline(file, line))
    {
        if (line.find("-----BEGIN") != std::string::npos)
        {
            inKey = true;
            continue;
        }
        if (line.find("-----END") != std::string::npos)
        {
            break;
        }
        if (inKey)
        {
            oss << line;
        }
    }
    file.close();
    return oss.str();
}

void RSACrypto::loadPrivateKeyFromPEM(const std::string &filename)
{
    std::string base64 = loadPEMFile(filename);

    std::string der;
    CryptoPP::StringSource ss1(
        base64, true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StringSink(der)
        )
    );

    CryptoPP::StringSource ss2(der, true);
    serverPrivateKey.Load(ss2);
}

void RSACrypto::loadPublicKeyFromPEM(const std::string &filename)
{
    std::string base64 = loadPEMFile(filename);

    std::string der;
    CryptoPP::StringSource ss1(
        base64, true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StringSink(der)
        )
    );

    CryptoPP::StringSource ss2(der, true);
    clientPublicKey.Load(ss2);
}

std::string RSACrypto::encrypt(const std::string &message) const
{
    CryptoPP::AutoSeededRandomPool rng;
    std::string cipher;

    CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(clientPublicKey);

    CryptoPP::StringSource ss(
        message, true,
        new CryptoPP::PK_EncryptorFilter(
            rng,
            encryptor,
            new CryptoPP::StringSink(cipher)
        )
    );

    return cipher;
}

std::string RSACrypto::decrypt(const std::string &ciphertext) const
{
    CryptoPP::AutoSeededRandomPool rng;
    std::string recovered;

    CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(serverPrivateKey);

    CryptoPP::StringSource ss(ciphertext, true,
        new CryptoPP::PK_DecryptorFilter(
            rng, 
            decryptor,
            new CryptoPP::StringSink(recovered)
        )
    );

    return recovered;
}
