#pragma once

#include "Constants.h"
#include "handler/IRequestHandler.h"

#include "infrastructure/OBuffer.h"

class IRequestHandler;

struct RequestResult
{
    RequestResult(const OBuffer& response, IRequestHandler* const newHandler);

    const OBuffer response;
    IRequestHandler* const newHandler;
};
