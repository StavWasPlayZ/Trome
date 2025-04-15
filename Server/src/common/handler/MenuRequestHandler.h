#pragma once

#include "IRequestHandler.h"

class MenuRequestHandler : public IRequestHandler
{
public:
    explicit MenuRequestHandler(const RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& info) const override;

    RequestResult handleRequest(const RequestInfo& info, const ProtocolRequest& request) const override;
};
