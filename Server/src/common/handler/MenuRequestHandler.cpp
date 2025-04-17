#include "MenuRequestHandler.h"

MenuRequestHandler::MenuRequestHandler(const RequestHandlerFactory &handlerFactory) : IRequestHandler(handlerFactory)
{}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo &request) const
{
    //TODO: Implement
    return true;
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& request) const
{
    //TODO: Implement
    return errorUnimplementedResult<MenuRequestHandler>();
}
