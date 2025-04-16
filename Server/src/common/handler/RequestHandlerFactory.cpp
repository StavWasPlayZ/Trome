#include "RequestHandlerFactory.h"

RequestHandlerFactory::RequestHandlerFactory(LoginManager &loginManager, const IDatabase& database) :
    loginManager(loginManager),
    m_database(database)
{}

LoginManager &RequestHandlerFactory::getLoginManager() const
{
    return this->loginManager;
}

const LoginRequestHandler *RequestHandlerFactory::createLoginRequestHandler() const
{
    return new LoginRequestHandler(*this);
}
