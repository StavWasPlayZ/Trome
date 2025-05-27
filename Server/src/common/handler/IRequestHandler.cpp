#include "IRequestHandler.h"

#include "RequestHandlerFactory.h"
#include "infrastructure/Client.h"

IRequestHandler::IRequestHandler(const RequestHandlerFactory &handlerFactory) :
    m_handlerFactory(handlerFactory)
{}

IRequestHandler::~IRequestHandler() = default;

LoggedUser &IRequestHandler::getUserByInfo(const RequestInfo &info) const
{
    return m_handlerFactory.getLoginManager().getUserByClient(info.client);
}

void IRequestHandler::dispatchNotification(const ProtocolNotification &notification,
                                           const std::vector<LoggedUser *> &users)
{
    for (const LoggedUser* user : users)
    {
        user->getClient().sendNotification(notification);
    }
}
