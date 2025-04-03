#pragma once

#include "Constants.h"

struct RequestInfo
{
    RequestInfo(const ProtocolCode id, const time_t receivalTime, const unsigned char* const buffer);

    const ProtocolCode id;
    const time_t receivalTime;

    //NOTE: We will not use a vector for that becase:
    //a. unconventional
    //b. already implememented everything asserting buffer is unsigned char
    const unsigned char* const buffer;
};
