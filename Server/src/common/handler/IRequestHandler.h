#pragma once

#include "codec/s2c/notification/Notification.h"
#include "infrastructure/request/RequestInfo.h"
#include "infrastructure/request/RequestResult.h"

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

    virtual bool isRequestRelevant(const RequestInfo& info) const = 0;
    virtual RequestResult handleRequest(const RequestInfo& info, const ProtocolRequest& request) const = 0;

    //TODO: Move to some utils class
    /**
     * Utility method to dispatch the provided notifications to all given users.
     */
    static void dispatchNotification(const ProtocolNotification &notification, const std::vector<LoggedUser *> &users);

protected:
    const RequestHandlerFactory& m_handlerFactory;

    /**
     * Utility method to get the current session user
     */
    LoggedUser & getUserByInfo(const RequestInfo& info) const;
};
