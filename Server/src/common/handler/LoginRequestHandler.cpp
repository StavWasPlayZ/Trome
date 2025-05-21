#include "LoginRequestHandler.h"

#include "codec/s2c/JsonResponsePacketSerializer.h"
#include "codec/s2c/Response.h"
#include "handler/RequestHandlerFactory.h"

LoginRequestHandler::LoginRequestHandler(const RequestHandlerFactory &handlerFactory) : IRequestHandler(handlerFactory)
{}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo &info) const
{
    //TODO: Check if client is already logged in (?)
    return (info.id == RequestCode::LOGIN) || (info.id == RequestCode::SIGNUP);
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& info, const ProtocolRequest& request) const
{
    switch (info.id)
    {
    case RequestCode::LOGIN: return login(info, static_cast<const LoginRequest&>(request));
    case RequestCode::SIGNUP: return signup(info, static_cast<const SignupRequest&>(request));

    default: throw std::runtime_error("Unexpected request ID");
    }
}

RequestResult LoginRequestHandler::login(const RequestInfo &info, const LoginRequest &request) const
{
    const ProtocolResponse *const response = this->m_handlerFactory.getLoginManager().login(info, request);

    if (response->id == ResponseCode::ERROR)
    {
        return RequestResult(
            JsonResponsePacketSerializer::serializeResponse(*static_cast<const ErrorResponse*>(response)),
            new LoginRequestHandler(*this)
        );
    }

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(*static_cast<const LoginResponse*>(response)),
        new MenuRequestHandler(this->m_handlerFactory)
    );
}

RequestResult LoginRequestHandler::signup(const RequestInfo &info, const SignupRequest &request) const
{
    const ProtocolResponse *const response = this->m_handlerFactory.getLoginManager().signup(info, request);

    if (response->id == ResponseCode::ERROR)
    {
        return RequestResult(
            JsonResponsePacketSerializer::serializeResponse(*static_cast<const LoginResponse*>(response)),
            new LoginRequestHandler(*this)
        );
    }

    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(*static_cast<const SignupResponse*>(response)),
        new MenuRequestHandler(this->m_handlerFactory)
    );
}
