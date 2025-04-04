#include "IRequestHandler.h"

#include "RequestHandlerFactory.h"

IRequestHandler::IRequestHandler(const RequestHandlerFactory &handlerFactory) :
    m_handlerFactory(handlerFactory)
{}
