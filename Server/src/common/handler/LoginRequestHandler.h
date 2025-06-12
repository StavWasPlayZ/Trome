#pragma once

#include "IRequestHandler.h"
#include "codec/c2s/request/Request.h"

class RequestHandlerFactory;  // Circle dependency

class LoginRequestHandler : public IRequestHandler
{
public:
    explicit LoginRequestHandler(const RequestHandlerFactory& handlerFactory);

    std::optional<ErrorStatus> isRequestRelevant(const RequestInfo& info) const override;

    RequestResult handleRequest(const RequestInfo& info, const ProtocolRequest& request) const override;

private:
    RequestResult login(const RequestInfo &info, const LoginRequest &request) const;
    RequestResult signup(const RequestInfo &info, const SignupRequest &request) const;
};
