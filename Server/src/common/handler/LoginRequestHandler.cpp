#include "LoginRequestHandler.h"

#include "infrastructure/codec/s2c/JsonResponsePacketSerializer.h"
#include "infrastructure/codec/s2c/Response.h"

bool LoginRequestHandler::isRequestRelevant(const RequestInfo &request) const
{
    //TODO: Check if client is already logged in (?)
    return (request.id == ProtocolCode::LOGIN) || (request.id == ProtocolCode::SIGNUP);
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo &request)
{
    //TODO: Actually implement
    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(LoginResponse(1)),
        this
    );
}
