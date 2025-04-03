#pragma once

#include "Constants.h"
#include "handler/IRequestHandler.h"

struct RequestResult
{
    RequestResult(const unsigned char* const response, const IRequestHandler* const newHandler);

    const unsigned char* const response;
    const IRequestHandler* const newHandler;
};
