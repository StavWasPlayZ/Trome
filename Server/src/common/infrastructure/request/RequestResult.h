#pragma once

#include "infrastructure/OBuffer.h"

class IRequestHandler;

struct RequestResult
{
    RequestResult(const OBuffer& response, const IRequestHandler* newHandler);

    const OBuffer response;
    const IRequestHandler* const newHandler;
};
