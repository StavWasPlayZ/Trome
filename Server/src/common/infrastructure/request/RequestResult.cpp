#include "RequestResult.h"

#include "handler/IRequestHandler.h"

RequestResult::RequestResult(const OBuffer &response, const IRequestHandler *const newHandler) :
    response(response),
    newHandler(newHandler)
{}
