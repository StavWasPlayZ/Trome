#include "OTP.h"

const std::string OTP::keyPath = "../../../src/common/infrastructure/cryptoAlgorithm/keys/OTPkey.key";
std::vector<unsigned char> OTP::key;

OTP::OTP()
{
    if (key.size() == 0)
    {
        key = ReadFileBytes(keyPath);
    }
}

std::string OTP::encrypt(const std::string &message) const
{
    std::string encrypted = "";
    int pos = 0;

    for (const char c : message)
    {
        encrypted += static_cast<char>(c ^ key.at(pos));
        pos++;
        pos %= key.size();
    }

    return encrypted;
}

std::string OTP::decrypt(const std::string &message) const
{
    std::string decrypted = "";
    int pos = 0;

    for (const char c : message)
    {
        decrypted += static_cast<char>(c ^ key.at(pos));
        pos++;
        pos %= key.size();
    }

    return decrypted;
}
