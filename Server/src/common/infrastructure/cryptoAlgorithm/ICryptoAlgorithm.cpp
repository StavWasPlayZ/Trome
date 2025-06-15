#include "ICryptoAlgorithm.h"

#include "exception/FileNotFoundException.h"

#include <fstream>

const std::string ICryptoAlgorithm::KEYS_PATH = "./resources/keys/";

ICryptoAlgorithm::~ICryptoAlgorithm() = default;

std::vector<unsigned char> ICryptoAlgorithm::readFileBytes(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        throw FileNotFoundException(path);
    }

    file.seekg(0, std::ios::end);
    const std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char *>(buffer.data()), size))
    {
        throw FileNotFoundException(path);
    }

    return buffer;
}

std::string ICryptoAlgorithm::readFileString(const std::string& path)
{
    const std::vector<unsigned char> bytes = readFileBytes(path);
    return std::string(reinterpret_cast<const char *>(bytes.data()), bytes.size());
}
