#include "RequestResult.h"

#include "handler/IRequestHandler.h"

RequestResult::RequestResult(const ProtocolResponse *const response, const IRequestHandler *const newHandler)
    : response(response), newHandler(newHandler)
{
}

RequestResult::~RequestResult()
{
    delete response;
}
