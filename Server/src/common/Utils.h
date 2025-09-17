#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <fstream>

#include "exception/FileNotFoundException.h"

namespace utils
{
    std::chrono::milliseconds getCurrTimeMillis();
    std::vector<unsigned char> readFileBytes(const std::string &path);
    std::string readFileString(const std::string &path);
}
