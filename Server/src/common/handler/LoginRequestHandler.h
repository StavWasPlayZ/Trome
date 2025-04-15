#pragma once

#include "IRequestHandler.h"
#include "codec/c2s/Request.h"

class RequestHandlerFactory;  // Circle dependency

class LoginRequestHandler : public IRequestHandler
{
public:
    explicit LoginRequestHandler(const RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& info) const override;

    RequestResult handleRequest(const RequestInfo& info, const ProtocolRequest& request) const override;

private:
    RequestResult login(const RequestInfo& context, const LoginRequest& request) const;
    RequestResult signup(const RequestInfo& context, const SignupRequest& request) const;
};
