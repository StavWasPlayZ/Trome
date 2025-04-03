#pragma once

#include "Constants.h"
#include "handler/IRequestHandler.h"

#include "infrastructure/OBuffer.h"

class IRequestHandler;

struct RequestResult
{
    RequestResult(const OBuffer& response, const IRequestHandler* const newHandler);

    const OBuffer response;
    const IRequestHandler* const newHandler;
};
