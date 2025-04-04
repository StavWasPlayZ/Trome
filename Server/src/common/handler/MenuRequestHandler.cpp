#include "MenuRequestHandler.h"

#include "codec/s2c/JsonResponsePacketSerializer.h"
#include "codec/s2c/Response.h"

MenuRequestHandler::MenuRequestHandler(const RequestHandlerFactory &handlerFactory) : IRequestHandler(handlerFactory)
{}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo &request) const
{
    //TODO: Implement
    return true;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo &request) const
{
    //TODO: Implement
    return errorUnimplementedResult<MenuRequestHandler>();
}
