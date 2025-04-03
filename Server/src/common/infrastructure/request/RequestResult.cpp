#include "RequestResult.h"

RequestResult::RequestResult(const unsigned char *const response, const IRequestHandler *const newHandler) :
    response(response),
    newHandler(newHandler)
{}
