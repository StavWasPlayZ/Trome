#pragma once

#include <string>
#include <vector>

class ICryptoAlgorithm
{
public:
    virtual ~ICryptoAlgorithm();

    virtual std::string encrypt(const std::string &message) const = 0;
    virtual std::string decrypt(const std::string &message) const = 0;

protected:
    static std::vector<unsigned char> ReadFileBytes(const std::string &path);
    static std::string ReadFileString(const std::string &path);
};