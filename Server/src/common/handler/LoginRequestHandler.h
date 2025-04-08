#pragma once

#include "IRequestHandler.h"

class RequestHandlerFactory;  // Circle calling so i need to put this here

class LoginRequestHandler : public IRequestHandler
{
public:
    LoginRequestHandler(const RequestHandlerFactory& handlerFactory);

    virtual bool isRequestRelevant(const RequestInfo& request) const override;
    virtual RequestResult handleRequest(const RequestInfo& request) const override;

private:
    RequestResult login(const RequestInfo& request) const;
    RequestResult signup(const RequestInfo& request) const;
};
