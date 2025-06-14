#include "IRequestHandler.h"

#include "RequestHandlerFactory.h"
#include "codec/s2c/response/ErrorResponse.h"
#include "infrastructure/Client.h"

IRequestHandler::IRequestHandler(const RequestHandlerFactory &handlerFactory) :
    m_handlerFactory(handlerFactory)
{}

IRequestHandler::~IRequestHandler() = default;

RequestResult IRequestHandler::getUserStatistics(const RequestInfo& info, const GetUserStatisticsRequest &request) const
{
    const StatisticsManager &sManager = m_handlerFactory.getStatisticsManager();
    const std::optional<UserStatistics> stats = sManager.getUserStatistics(request.userId);

    if (!stats.has_value())
    {
        return RequestResult(new ErrorResponse(ErrorStatus::UNKNOWN_RESOURCE, info.id));
    }

    return RequestResult(
        new GetUserStatisticsResponse(
            sManager.getUserStatistics(request.userId).value()
        )
    );
}

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
