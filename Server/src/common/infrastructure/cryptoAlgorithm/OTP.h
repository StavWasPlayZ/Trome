#pragma once

#include "CipherEncryption.h"
#include <vector>

class OTP : public ICryptoAlgorithm
{
public:
	OTP();
	std::string encrypt(const std::string &message) const override;
	std::string decrypt(const std::string &message) const override;

private:
    static const std::string keyPath;
    static std::vector<unsigned char> key;
};