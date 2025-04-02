#pragma once

#include <iostream>

enum class ProtocolCode : unsigned int
{
    LOGIN = 1,
    SIGNUP = 2,
    ERROR = 400,
};

struct ProtocolJsonKeys
{
    static const std::string STATUS;
    static const std::string MESSAGE;
};

// sizes
constexpr unsigned int SIZE_CODE = sizeof(unsigned char);
constexpr unsigned int SIZE_JSON_LEN = 4 * sizeof(unsigned char);