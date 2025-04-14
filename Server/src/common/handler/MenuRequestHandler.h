#pragma once

#include "IRequestHandler.h"

class MenuRequestHandler : public IRequestHandler
{
public:
    explicit MenuRequestHandler(const RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& request) const override;

    RequestResult handleRequest(const RequestInfo& request) const override;
};
