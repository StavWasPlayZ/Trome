#include "AES.h"

#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/base64.h>
#include <cryptopp/cryptlib.h>

const std::string AES::keyPath = "../../../src/common/infrastructure/cryptoAlgorithm/keys/AESkey.key";
const std::string AES::ivPath = "../../../src/common/infrastructure/cryptoAlgorithm/keys/AESiv.key";

std::vector<unsigned char> AES::key;
std::vector<unsigned char> AES::iv;

AES::AES() : ICryptoAlgorithm()
{
    if (key.size() == 0)
    {
        key = ReadFileBytes(keyPath);
    }

    if (iv.size() == 0)
    {
        iv = ReadFileBytes(ivPath);
    }
}

std::string AES::encrypt(const std::string &message) const
{
    std::string encrypted;
    
    auto aes = CryptoPP::AES::Encryption(key.data(), key.size());
    auto aes_cbc = CryptoPP::CBC_Mode_ExternalCipher::Encryption(aes, iv.data());
    
    CryptoPP::StringSource ss(
        message,
        true,
        new CryptoPP::StreamTransformationFilter(
            aes_cbc,
            new CryptoPP::Base64Encoder(
                new CryptoPP::StringSink(encrypted)
            )
        )
    );
    
    return encrypted;
}

std::string AES::decrypt(const std::string &message) const
{
    std::string decrypted;

    auto aes = CryptoPP::AES::Decryption(key.data(), key.size());
    auto aes_cbc = CryptoPP::CBC_Mode_ExternalCipher::Decryption(aes, iv.data());

    CryptoPP::StringSource ss(
        message,
        true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StreamTransformationFilter(
                aes_cbc, 
                new CryptoPP::StringSink(decrypted)
            )
        )
    );

    return decrypted;
}
