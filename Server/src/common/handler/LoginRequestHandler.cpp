#include "LoginRequestHandler.h"

#include "codec/s2c/JsonResponsePacketSerializer.h"
#include "codec/s2c/Response.h"

LoginRequestHandler::LoginRequestHandler(const RequestHandlerFactory &handlerFactory) : IRequestHandler(handlerFactory)
{}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo &request) const
{
    //TODO: Check if client is already logged in (?)
    return (request.id == ProtocolCode::LOGIN) || (request.id == ProtocolCode::SIGNUP);
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo &request) const
{
    if (request.id == ProtocolCode::LOGIN)
    {
        return login(request);
    }
    else
    {
        // We already checked that it must be either of the 2.
        return signup(request);
    }
}

RequestResult LoginRequestHandler::login(const RequestInfo &request) const
{
    //TODO: Implement after LoginManager is complete
    return errorUnimplementedResult<LoginRequestHandler>();
}

RequestResult LoginRequestHandler::signup(const RequestInfo &request) const
{
    //TODO: Implement after LoginManager is complete
    return errorUnimplementedResult<LoginRequestHandler>();
}
