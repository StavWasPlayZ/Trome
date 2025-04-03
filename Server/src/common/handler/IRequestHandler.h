#pragma once

#include "infrastructure/request/RequestInfo.h"
#include "infrastructure/request/RequestResult.h"

struct RequestResult;

class IRequestHandler
{
public:
    virtual bool isRequestRelevant(const RequestInfo& request) const = 0;
    virtual RequestResult handleRequest(const RequestInfo& request) const = 0;
};
