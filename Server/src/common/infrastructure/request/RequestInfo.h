#pragma once

#include <chrono>
#include <nlohmann/json.hpp>

#include "Constants.h"

struct RequestInfo
{
    RequestInfo(const ProtocolCode id, const std::time_t receivalTime, const nlohmann::json& data);

    const ProtocolCode id;
    const std::time_t receivalTime;

    //NOTE: We will not use a buffer *vector* becase:
    //a. unconventional
    //b. already implememented everything asserting buffer is unsigned char
    //
    // And, if we already have an entire struct JUST for the request...
    // Why not just detail the request INSIDE the buffer instead of just providing the buffer?

    const nlohmann::json data;
};
