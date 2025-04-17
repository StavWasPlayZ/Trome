#include "IRequestHandler.h"

IRequestHandler::IRequestHandler(const RequestHandlerFactory &handlerFactory) :
    m_handlerFactory(handlerFactory)
{}

IRequestHandler::~IRequestHandler() = default;
