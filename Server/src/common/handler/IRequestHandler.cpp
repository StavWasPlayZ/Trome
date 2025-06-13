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


void IRequestHandler::dispatchNotification(
    const std::optional<std::function<const IRequestHandler *(const LoggedUser *)>> &factory,
    const std::vector<LoggedUser *> &users,
    const std::optional<ProtocolNotification> &notification,
    const std::optional<const LoggedUser *> &excluded
)
{
    for (const LoggedUser *user : users)
    {
        if (excluded.has_value() && *excluded.value() == *user)
            continue;

        Client& client = user->getClient();

        if (factory.has_value())
        {
            client.setRequestHandlerSafe(factory.value()(user));
        }

        if (notification.has_value())
        {
            client.sendNotification(notification.value());
        }
    }
}

void IRequestHandler::dispatchNotification(const ProtocolNotification &notification,
                                           const std::vector<LoggedUser *> &users,
                                           const std::optional<const LoggedUser *> &excluded)
{
    dispatchNotification(std::nullopt, users, notification, excluded);
}

void IRequestHandler::setRequestHandlers(const std::function<const IRequestHandler *(const LoggedUser *)> &factory,
                                         const std::vector<LoggedUser *> &users,
                                         const std::optional<ProtocolNotification> &notification,
                                         const std::optional<const LoggedUser *> &excluded)
{
    dispatchNotification(factory, users, notification, excluded);
}
