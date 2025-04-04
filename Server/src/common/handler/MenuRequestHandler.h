#pragma once

#include "IRequestHandler.h"

class MenuRequestHandler : public IRequestHandler
{
public:
    MenuRequestHandler(const RequestHandlerFactory& handlerFactory);

    virtual bool isRequestRelevant(const RequestInfo& request) const override;
    virtual RequestResult handleRequest(const RequestInfo& request) const override;
};
