#include "AES.h"

static const std::vector<unsigned char> key = 
{
    0x2A, 0x8D, 0xB4, 0x6C, 0xC1, 0xFA, 0x12, 0x34,
    0x7B, 0x9E, 0x56, 0xAF, 0x44, 0xE1, 0x8C, 0x3D
};

static const std::vector<unsigned char> iv = {
    0x1F, 0x26, 0x73, 0x9A, 0x4B, 0xD5, 0xCE, 0x22,
    0x39, 0x81, 0x67, 0xF4, 0x0C, 0xAB, 0x58, 0xE7
};

AES::AES() : ICryptoAlgorithm()
{
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
