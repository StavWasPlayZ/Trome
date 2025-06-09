#include "CipherEncryption.h"

CipherEncryption::CipherEncryption() : ICryptoAlgorithm()
{}

std::string CipherEncryption::encrypt(const std::string &message) const
{
    std::string encrypted = "";
    for (char c : message)
    {
        encrypted += static_cast<char>(c + key);
    }

    return encrypted;
}

std::string CipherEncryption::decrypt(const std::string &message) const
{
    std::string dencrypted = "";
    for (char c : message)
    {
        dencrypted += static_cast<char>(c - key);
    }

    return dencrypted;
}
