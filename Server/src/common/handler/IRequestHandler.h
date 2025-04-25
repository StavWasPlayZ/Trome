#pragma once

#include "infrastructure/request/RequestInfo.h"

class LoggedUser;
struct ProtocolRequest;
struct RequestInfo;
struct RequestResult;
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

    /**
     * T - The current handler type
     */
    template <typename T>
    RequestResult errorUnimplementedResult() const;

protected:
    const RequestHandlerFactory& m_handlerFactory;

    /**
    * Utility method to get the current session user
    */
    LoggedUser & getUserByInfo(const RequestInfo& info) const;
};


#include "IRequestHandler.tpp"
