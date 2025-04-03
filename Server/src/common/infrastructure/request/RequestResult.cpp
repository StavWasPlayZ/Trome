#include "RequestResult.h"

RequestResult::RequestResult(const OBuffer &response, IRequestHandler *const newHandler) :
    response(response),
    newHandler(newHandler)
{}
