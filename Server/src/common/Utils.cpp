#include "Utils.h"

std::chrono::milliseconds utils::getCurrTimeMillis()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    );
}

std::vector<unsigned char> utils::readFileBytes(const std::string &path)
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

std::string utils::readFileString(const std::string &path)
{
    std::ifstream file(path);
    if (!file)
    {
        throw FileNotFoundException(path);
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}
