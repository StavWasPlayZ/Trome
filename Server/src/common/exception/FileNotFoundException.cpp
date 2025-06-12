#include "FileNotFoundException.h"

FileNotFoundException::FileNotFoundException(const std::string &path) : message("file at: " + path + " not found")
{}

const char *FileNotFoundException::what() const noexcept
{
    return message.c_str();
}
