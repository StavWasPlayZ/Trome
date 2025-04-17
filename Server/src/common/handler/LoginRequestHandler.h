#pragma once

#include "IRequestHandler.h"

class RequestHandlerFactory;  // Circle dependency

class LoginRequestHandler : public IRequestHandler
{
public:
    explicit LoginRequestHandler(const RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& request) const override;

    RequestResult handleRequest(const RequestInfo& request) const override;

private:
    RequestResult login(const RequestInfo& request) const;
    RequestResult signup(const RequestInfo& request) const;
};
