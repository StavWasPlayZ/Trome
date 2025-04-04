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
    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(ErrorResponse("Unimplemented server-side")),
        new MenuRequestHandler(*this)
    );
}
