#include "RequestResult.h"

#include "handler/codec/s2c/notification/Notification.h"
#include "handler/IRequestHandler.h"

NotificationPayload::NotificationPayload(const ProtocolNotification *const notification,
                                         const std::vector<const Client *> &clients) :
    notification(notification),
    clients(clients)
{}

NotificationPayload::~NotificationPayload()
{
    delete notification;
}

RequestResult::RequestResult(const ProtocolResponse *const response, const IRequestHandler *const newHandler,
        const std::optional<const NotificationPayload*> &notificationPayload) :
    response(response),
    newHandler(newHandler),
    notificationPayload(notificationPayload)
{}
