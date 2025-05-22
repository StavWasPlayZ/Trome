#include "LoginRequestHandler.h"

#include "codec/s2c/response/Response.h"
#include "codec/s2c/response/JsonResponsePacketSerializer.h"
#include "handler/RequestHandlerFactory.h"

LoginRequestHandler::LoginRequestHandler(const RequestHandlerFactory &handlerFactory) : IRequestHandler(handlerFactory)
{}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo &info) const
{
    return ((info.id == RequestCode::LOGIN) || (info.id == RequestCode::SIGNUP))
        && !this->m_handlerFactory.getLoginManager().isLoggedIn(info.client);
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
        const OBuffer serialized = JsonResponsePacketSerializer::serializeResponse(*static_cast<const ErrorResponse*>(response));
        delete response;

        return RequestResult(serialized, new LoginRequestHandler(*this));
    }

    const OBuffer serialized = JsonResponsePacketSerializer::serializeResponse(*static_cast<const LoginResponse*>(response));
    delete response;

    return RequestResult(serialized, new MenuRequestHandler(this->m_handlerFactory));
}

RequestResult LoginRequestHandler::signup(const RequestInfo &info, const SignupRequest &request) const
{
    const ProtocolResponse *const response = this->m_handlerFactory.getLoginManager().signup(info, request);

    if (response->id == ResponseCode::ERROR)
    {
        const OBuffer serialized = JsonResponsePacketSerializer::serializeResponse(*static_cast<const ErrorResponse*>(response));
        delete response;

        return RequestResult(serialized, new LoginRequestHandler(*this));
    }

    const OBuffer serialized = JsonResponsePacketSerializer::serializeResponse(*static_cast<const SignupResponse*>(response));
    delete response;

    return RequestResult(serialized, new MenuRequestHandler(this->m_handlerFactory));
}
