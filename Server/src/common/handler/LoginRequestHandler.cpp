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
    switch (request.id)
    {
    case ProtocolCode::LOGIN: return login(request);
    case ProtocolCode::SIGNUP: return signup(request);

    default: throw std::runtime_error("Unexpected request ID");
    }
}

RequestResult LoginRequestHandler::login(const RequestInfo &request) const
{
    LoginResponse response = this->m_handlerFactory.getLoginManager().login(
        request.data.at("username"),
        request.data.at("password")
    );

    if (response.status != LoginStatus::SUCCESS)
    {
        return RequestResult(
            JsonResponsePacketSerializer::serializeResponse(response),
            new LoginRequestHandler(*this)
        );
    }

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(response),
        new LoginRequestHandler(*this)
    ); // change this later to have a MenuRequestHandler instead of this
}

RequestResult LoginRequestHandler::signup(const RequestInfo &request) const
{
    SignupResponse response = this->m_handlerFactory.getLoginManager().signup(
        request.data.at("username"),
        request.data.at("password"),
        request.data.at("email")
    );

    if (response.status != SignupStatus::SUCCESS)
    {
        return RequestResult(
            JsonResponsePacketSerializer::serializeResponse(response),
            new LoginRequestHandler(*this)
        );
    }

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(response),
        new LoginRequestHandler(*this)
    ); // change this later to have a MenuRequestHandler instead of this
}
