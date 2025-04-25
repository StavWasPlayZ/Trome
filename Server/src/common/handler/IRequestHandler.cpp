#include "IRequestHandler.h"

#include "RequestHandlerFactory.h"

IRequestHandler::IRequestHandler(const RequestHandlerFactory &handlerFactory) :
    m_handlerFactory(handlerFactory)
{}

IRequestHandler::~IRequestHandler() = default;

LoggedUser &IRequestHandler::getUserByInfo(const RequestInfo &info) const
{
    return m_handlerFactory.getLoginManager().getUserByClient(info.client);
}
