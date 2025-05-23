#pragma once

#include <vector>
#include <optional>

struct ProtocolResponse;
class IRequestHandler;
struct ProtocolNotification;
class LoggedUser;
struct NotificationPayload;


struct RequestResult
{
    RequestResult(const ProtocolResponse* response, const IRequestHandler* newHandler,
        const std::optional<const NotificationPayload*>& notificationPayload = std::nullopt);

    /**
     * NOTE: THIS RESOURCE MUST BE FREED
     */
    const ProtocolResponse* response;
    /**
     * NOTE: THIS RESOURCE MUST BE FREED
     */
    const IRequestHandler* const newHandler;

    /**
     * NOTE: THIS RESOURCE MUST BE FREED
     */
    const std::optional<const NotificationPayload*> notificationPayload;
};


struct NotificationPayload
{
    NotificationPayload(const ProtocolNotification* notification, const std::vector<LoggedUser*>& clients);
    ~NotificationPayload();

    const ProtocolNotification* notification;

    // Made of LoggedUser and not Client for ease of use
    /**
     * The clients to send the notification to.
     */
    const std::vector<LoggedUser*> clients;
};
