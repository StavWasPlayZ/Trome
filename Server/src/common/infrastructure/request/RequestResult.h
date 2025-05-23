#pragma once

#include "handler/codec/s2c/response/Response.h"

class IRequestHandler;

struct RequestResult
{
    RequestResult(const ProtocolResponse* response, const IRequestHandler* newHandler);
    ~RequestResult();

    const ProtocolResponse* response;
    const IRequestHandler* const newHandler;
};
