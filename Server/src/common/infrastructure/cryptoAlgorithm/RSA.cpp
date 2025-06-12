#include "RSA.h"

const std::string RSA::privateServerPath = "../../../src/common/infrastructure/cryptoAlgorithm/keys/privateServer.key";
const std::string RSA::publicClientPath = "../../../src/common/infrastructure/cryptoAlgorithm/keys/publicClient.key";

std::string RSA::privateServerKeyStr = "";
std::string RSA::publicClientKeyStr = "";

CryptoPP::RSA::PrivateKey RSA::serverPrivateKey;
CryptoPP::RSA::PublicKey RSA::clientPublicKey;

CryptoPP::AutoSeededRandomPool rng;

RSA::RSA() : ICryptoAlgorithm()
{
    if (RSA::privateServerKeyStr == "")
    {
        RSA::privateServerKeyStr = ICryptoAlgorithm::ReadFileString(RSA::privateServerPath);
        RSA::LoadPrivateKey(RSA::privateServerKeyStr);
    }
    if (RSA::publicClientKeyStr == "")
    {
        RSA::publicClientKeyStr = ICryptoAlgorithm::ReadFileString(RSA::publicClientPath);
        RSA::LoadPublicKey(RSA::publicClientKeyStr);
    }
}

std::string RSA::encrypt(const std::string &message) const
{
    std::string encrypted = "";
    
    CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(RSA::clientPublicKey);
    CryptoPP::StringSource ss(
        message, true,
            new CryptoPP::PK_EncryptorFilter(
            RSA::rng, 
            encryptor, 
            new CryptoPP::StringSink(encrypted)
        )
    );

    return encrypted;
}

std::string RSA::decrypt(const std::string &message) const
{
    std::string decrypted = "";
    CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(RSA::serverPrivateKey);
    CryptoPP::StringSource ss(
        message, true,
        new CryptoPP::PK_DecryptorFilter(
            RSA::rng,
            decryptor,
            new CryptoPP::StringSink(decrypted)
        )
    );

    return decrypted;
}

void RSA::LoadPublicKey(const std::string &keyStr)
{
    CryptoPP::StringSource ss(keyStr, true);
    RSA::clientPublicKey.Load(ss);
}

void RSA::LoadPrivateKey(const std::string &keyStr)
{
    CryptoPP::StringSource ss(keyStr, true);
    RSA::serverPrivateKey.Load(ss);
}
