#include "CipherEncryption.h"

std::string CipherEncryption::encrypt(const std::string &message) const
{
    std::string encrypted = "";
    for (const char c : message)
    {
        encrypted += static_cast<char>(c + key);
    }

    return encrypted;
}

std::string CipherEncryption::decrypt(const std::string &message) const
{
    std::string decrypted = "";
    for (const char c : message)
    {
        decrypted += static_cast<char>(c - key);
    }

    return decrypted;
}
