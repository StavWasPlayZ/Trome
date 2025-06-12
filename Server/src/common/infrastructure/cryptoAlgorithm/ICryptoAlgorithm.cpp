#include "ICryptoAlgorithm.h"

ICryptoAlgorithm::ICryptoAlgorithm()
{
}

static std::vector<unsigned char> ReadFileBytes(const std::string &path)
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
