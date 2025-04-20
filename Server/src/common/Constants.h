#pragma once

#include <iostream>

struct ProtocolJsonKeys
{
    static const std::string STATUS;
    static const std::string USER_ID;
    static const std::string MESSAGE;
    static const std::string ROOMS;
};

// sizes
constexpr unsigned int SIZE_CODE = sizeof(char);
constexpr unsigned int SIZE_JSON_LEN = sizeof(int);