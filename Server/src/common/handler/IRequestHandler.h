#pragma once

#include "infrastructure/request/RequestInfo.h"

struct RequestResult;

class IRequestHandler
{
public:
    virtual bool isRequestRelevant(const RequestInfo& request) const = 0;
    virtual RequestResult handleRequest(const RequestInfo& request) const = 0;

    /**
     * T - The current handler type
     */
    template <typename T>
    RequestResult errorUnimplementedResult() const;
};


#include "IRequestHandler.tpp"
