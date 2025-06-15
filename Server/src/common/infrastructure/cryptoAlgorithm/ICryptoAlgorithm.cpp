#include "ICryptoAlgorithm.h"

ICryptoAlgorithm::~ICryptoAlgorithm() = default;

std::vector<unsigned char> ICryptoAlgorithm::ReadFileBytes(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
    {
        throw FileNotFoundException(path);
    }

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char *>(buffer.data()), size))
    {
        throw FileNotFoundException(path);
    }

    return buffer;
}

std::string ICryptoAlgorithm::ReadFileString(const std::string& path)
{
    std::vector<unsigned char> bytes = ICryptoAlgorithm::ReadFileBytes(path);
    return std::string(reinterpret_cast<const char *>(bytes.data()), bytes.size());
}
