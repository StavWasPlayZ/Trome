#include "MenuRequestHandler.h"

#include "codec/s2c/JsonResponsePacketSerializer.h"
#include "codec/s2c/Response.h"

bool MenuRequestHandler::isRequestRelevant(const RequestInfo &request) const
{
    //TODO: Implement
    return true;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo &request) const
{
    //TODO: Implement
    return errorServerUnimplemented<MenuRequestHandler>();
}
