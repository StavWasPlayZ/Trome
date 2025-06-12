#include "AES.h"

const std::string AES::keyPath = "../../../src/common/infrastructure/cryptoAlgorithm/keys/AESkey.key";
const std::string AES::ivPath = "../../../src/common/infrastructure/cryptoAlgorithm/keys/AESiv.key";

std::vector<unsigned char> AES::key;

std::vector<unsigned char> AES::iv;

AES::AES() : ICryptoAlgorithm()
{
    if (AES::key.size() == 0)
    {
        key = ICryptoAlgorithm::ReadFileBytes(AES::keyPath);
    }

    if (AES::iv.size() == 0)
    {
        iv = ICryptoAlgorithm::ReadFileBytes(AES::ivPath);
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
