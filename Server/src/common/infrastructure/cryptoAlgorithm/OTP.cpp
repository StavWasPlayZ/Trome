#include "OTP.h"

const std::string OTP::keyPath = KEYS_PATH + "OTPkey.key";
std::vector<unsigned char> OTP::key;

OTP::OTP()
{
    if (key.size() == 0)
    {
        key = utils::readFileBytes(keyPath);
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
