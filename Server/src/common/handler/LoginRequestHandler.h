#pragma once

#include "handler/IRequestHandler.h"

class LoginRequestHandler : public IRequestHandler
{
public:
    virtual bool isRequestRelevant(const RequestInfo& request) const override;
    virtual RequestResult handleRequest(const RequestInfo& request) const override;
};
