#pragma once

#include <chrono>
#include <nlohmann/json.hpp>

enum class RequestCode : unsigned char;
class Client;

struct RequestInfo
{
    RequestInfo(const Client& client, RequestCode id, std::time_t receivalTime, const nlohmann::json& data);

    const Client& client;

    const RequestCode id;
    const std::time_t receivalTime;

    //NOTE: We will not use a buffer *vector* because:
    // a. unconventional
    // b. already implemented everything asserting buffer is unsigned char
    //
    // And, if we already have an entire struct JUST for the request...
    // Why not just detail the request INSIDE the buffer instead of just providing the buffer?

    const nlohmann::json data;
};
