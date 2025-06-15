#include "RSA.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include <cryptopp/base64.h>
#include <cryptopp/files.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>

const std::string RSACrypto::serverPrivateKeyPath =  "../../../src/common/infrastructure/cryptoAlgorithm/keys/privateServer.pem";
const std::string RSACrypto::clientPublicKeyPath = "../../../src/common/infrastructure/cryptoAlgorithm/keys/publicClient.pem";

CryptoPP::RSA::PrivateKey RSACrypto::serverPrivateKey;
CryptoPP::RSA::PublicKey RSACrypto::clientPublicKey;
bool RSACrypto::keysLoaded = false;

RSACrypto::RSACrypto()
{
    if (!keysLoaded)
    {
        loadPrivateKeyFromPEM(serverPrivateKeyPath);
        loadPublicKeyFromPEM(clientPublicKeyPath);
        keysLoaded = true;
    }
}

std::string RSACrypto::loadPEMFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        throw FileNotFoundException(filename);
    }

    std::ostringstream oss;
    std::string line;
    bool inKey = false;

    while (std::getline(file, line))
    {
        if (line.find("-----BEGIN") != std::string::npos)
        {
            inKey = true;
            continue;
        }

        if (line.find("-----END") != std::string::npos)
        {
            break;
        }

        if (inKey)
        {
            oss << line;
        }
    }

    return oss.str();
}

void RSACrypto::loadPrivateKeyFromPEM(const std::string &filename)
{
    std::string base64 = loadPEMFile(filename);

    std::string der;
    CryptoPP::StringSource ss1(
        base64, true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StringSink(der)
        )
    );

    CryptoPP::StringSource ss2(der, true);
    serverPrivateKey.Load(ss2);
}

void RSACrypto::loadPublicKeyFromPEM(const std::string &filename)
{
    std::string base64 = loadPEMFile(filename);

    std::string der;
    CryptoPP::StringSource ss1(
        base64, true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StringSink(der)
        )
    );

    CryptoPP::StringSource ss2(der, true);
    clientPublicKey.Load(ss2);
}

size_t RSACrypto::getMaxPlaintextSize() const
{
    // Key size in bytes
    size_t keySize = clientPublicKey.GetModulus().ByteCount();

    // Max plaintext size for RSAES_OAEP with SHA1
    return keySize - 2 * hashLen - 2;
}

size_t RSACrypto::getEncryptedTextSize() const
{
    // Encryptedext size = RSA modulus size in bytes
    return clientPublicKey.GetModulus().ByteCount();
}

std::string RSACrypto::encrypt(const std::string &message) const
{
    CryptoPP::AutoSeededRandomPool rng;

    size_t maxPlaintextLen = getMaxPlaintextSize();

    std::string encryptedText = "";

    for (size_t pos = 0; pos < message.size(); pos += maxPlaintextLen)
    {
        size_t chunkSize = std::min(
            maxPlaintextLen,
            message.size() - pos
        );

        std::string chunk = message.substr(pos, chunkSize);

        std::string encryptedChunck = "";

        CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(clientPublicKey);

        CryptoPP::StringSource ss(
            chunk, true,
            new CryptoPP::PK_EncryptorFilter(
                rng,
                encryptor,
                new CryptoPP::StringSink(encryptedChunck)
            )
        );

        encryptedText.append(encryptedChunck);
    }

    std::string base64Encoded = "";
    CryptoPP::StringSource ss2(
        encryptedText, true,
        new CryptoPP::Base64Encoder(
            new CryptoPP::StringSink(base64Encoded),
            false
            )
    );

    return base64Encoded;
}

std::string RSACrypto::decrypt(const std::string &message) const
{
    CryptoPP::AutoSeededRandomPool rng;

    std::string ciphertextRaw;
    CryptoPP::StringSource ss1(
        message, true,
        new CryptoPP::Base64Decoder(
            new CryptoPP::StringSink(ciphertextRaw)
        )
    );

    size_t encryptedLen = getEncryptedTextSize();

    if (ciphertextRaw.size() % encryptedLen != 0)
    {
        throw std::runtime_error("Invalid ciphertext length (not multiple of RSA ciphertext block size).");
    }

    std::string decryptedText = "";

    CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(serverPrivateKey);

    for (size_t pos = 0; pos < ciphertextRaw.size(); pos += encryptedLen)
    {
        std::string chunk = ciphertextRaw.substr(pos, encryptedLen);

        std::string decryptedChunck = "";

        CryptoPP::StringSource ss(
            chunk, true,
            new CryptoPP::PK_DecryptorFilter(
                rng, decryptor,
                new CryptoPP::StringSink(decryptedChunck)
            )
        );

        decryptedText.append(decryptedChunck);
    }

    return decryptedText;
}
