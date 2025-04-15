#include "LoginRequestHandler.h"

#include "codec/s2c/JsonResponsePacketSerializer.h"
#include "codec/s2c/Response.h"
#include "handler/RequestHandlerFactory.h"

LoginRequestHandler::LoginRequestHandler(const RequestHandlerFactory &handlerFactory) : IRequestHandler(handlerFactory)
{}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo &info) const
{
    //TODO: Check if client is already logged in (?)
    return (info.id == ProtocolCode::LOGIN) || (info.id == ProtocolCode::SIGNUP);
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& info, const ProtocolRequest& request) const
{
    switch (info.id)
    {
    case ProtocolCode::LOGIN: return login(info, (LoginRequest&) request);
    case ProtocolCode::SIGNUP: return signup(info, (SignupRequest&) request);

    default: throw std::runtime_error("Unexpected request ID");
    }
}

RequestResult LoginRequestHandler::login(const RequestInfo& context, const LoginRequest& request) const
{
    const LoginResponse response = this->m_handlerFactory.getLoginManager().login(context, request);

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

RequestResult LoginRequestHandler::signup(const RequestInfo& context, const SignupRequest& request) const
{
    const SignupResponse response = this->m_handlerFactory.getLoginManager().signup(context, request);

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
