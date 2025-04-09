#include "LoginRequestHandler.h"

#include "codec/s2c/JsonResponsePacketSerializer.h"
#include "codec/s2c/Response.h"
#include "handler/RequestHandlerFactory.h"

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
    else if (request.id == ProtocolCode::SIGNUP)
    {
        // We already checked that it must be either of the 2.
        return signup(request);
    }
}

RequestResult LoginRequestHandler::login(const RequestInfo &request) const
{
    LoginResponse response = this->m_handlerFactory.getLoginManager().login(
        request.data.at("username"),
        request.data.at("password")
    );

    if (response.status == LoginStatus::FAILED_INVALID_CREDENTIALS)
    {
        return RequestResult(JsonResponsePacketSerializer::serializeResponse(response), this);
    }

    return RequestResult(JsonResponsePacketSerializer::serializeResponse(response), this); // change this later to have a MenuRequestHandler instead of this
}

RequestResult LoginRequestHandler::signup(const RequestInfo &request) const
{
    SignupResponse response = this->m_handlerFactory.getLoginManager().signup(
        request.data.at("username"),
        request.data.at("password"),
        request.data.at("email")
    );

    if (response.status == SignupStatus::FAILED_INTERNAL_ERROR)
    {
        return RequestResult(JsonResponsePacketSerializer::serializeResponse(response), this);
    }

    return RequestResult(JsonResponsePacketSerializer::serializeResponse(response), this); // change this later to have a MenuRequestHandler instead of this
}
