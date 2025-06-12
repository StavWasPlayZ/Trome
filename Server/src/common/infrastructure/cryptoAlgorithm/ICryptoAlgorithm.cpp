#include "ICryptoAlgorithm.h"

ICryptoAlgorithm::ICryptoAlgorithm()
{
}

std::vector<unsigned char> ICryptoAlgorithm::ReadFileBytes(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file)
        return {}; // Return empty vector if file can't be opened

    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    if (!file.read(reinterpret_cast<char *>(buffer.data()), size))
    {
        return {}; // Return empty vector if read fails
    }

    return buffer;
}
