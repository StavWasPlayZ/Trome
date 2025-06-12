#pragma once

#include <string>
#include <vector>
#include <fstream>

#include "exception/FileNotFoundException.h"

class ICryptoAlgorithm
{
public:
    ICryptoAlgorithm();
    virtual std::string encrypt(const std::string &message) const = 0;
    virtual std::string decrypt(const std::string &message) const = 0;

protected:
    static std::vector<unsigned char> ReadFileBytes(const std::string &path);
};