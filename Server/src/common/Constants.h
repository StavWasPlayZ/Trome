#pragma once

#include <iostream>

enum class ProtocolCode : unsigned char
{
    ERROR = 0,
    LOGIN = 1,
    SIGNUP = 2,
    OK = 255
};

struct ProtocolJsonKeys
{
    static const std::string STATUS;
    static const std::string USER_ID;
    static const std::string MESSAGE;
};

// sizes
constexpr unsigned int SIZE_CODE = sizeof(char);
constexpr unsigned int SIZE_JSON_LEN = sizeof(int);