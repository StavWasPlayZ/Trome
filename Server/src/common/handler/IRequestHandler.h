#pragma once

#include "codec/s2c/notification/Notification.h"
#include "infrastructure/request/RequestInfo.h"
#include "infrastructure/request/RequestResult.h"

struct GetUserStatisticsRequest;
enum class ErrorStatus : unsigned int;
class LoggedUser;
struct ProtocolRequest;
struct RequestInfo;
class RequestHandlerFactory;
class Client;

// This is technically an abstract class the way it's implemented lmao
// But we don't mindddd
class IRequestHandler
{
public:
    explicit IRequestHandler(const RequestHandlerFactory& handlerFactory);
    virtual ~IRequestHandler();

    /**
     * Checks whether the provided request is relevant to the current state.
     * If not, returns an error status.
     */
    virtual std::optional<ErrorStatus> isRequestRelevant(const RequestInfo& info) const = 0;
    virtual RequestResult handleRequest(const RequestInfo& info, const ProtocolRequest& request) const = 0;


    //TODO: Move to some utils class
    /**
     * Utility method to dispatch the provided notifications to all given users, excluding the provided one (if any).
     */
    static void dispatchNotification(
        const ProtocolNotification &notification,
        const std::vector<LoggedUser *> &users,
        const std::optional<const LoggedUser*>& excluded = std::nullopt
    );

    /**
     * Sets the request handlers of all provided clients to the given handler, and sends them the given notification.
     *
     * The notification is required to alert the clients of the change in state.
     */
    static void setRequestHandlers(
        const std::function<const IRequestHandler*(const LoggedUser*)>& factory,
        const std::vector<LoggedUser *> &users,
        const std::optional<ProtocolNotification> &notification = std::nullopt,
        const std::optional<const LoggedUser*>& excluded = std::nullopt
    );

protected:
    const RequestHandlerFactory& m_handlerFactory;

    RequestResult getUserStatistics(const RequestInfo& info, const GetUserStatisticsRequest &request) const;

    /**
     * Utility method to get the current session user
     */
    LoggedUser & getUserByInfo(const RequestInfo& info) const;

private:
    /**
     * Does 2 things:
     * - Sets the request handlers of all provided users
     * - Sends a notification to all provided users
     *
     * Excludes the provided user, if provided.
     */
    static void dispatchNotification(
        const std::optional<std::function<const IRequestHandler*(const LoggedUser*)>>& factory,
        const std::vector<LoggedUser *> &users,
        const std::optional<ProtocolNotification> &notification,
        const std::optional<const LoggedUser*>& excluded = std::nullopt
    );
};
