#include "RequestResult.h"

#include "handler/codec/s2c/response/Response.h"

RequestResult::RequestResult(const ProtocolResponse *const response,
        const std::optional<const IRequestHandler *>& newHandler) :
    response(response),
    newHandler(newHandler)
{}

RequestResult::~RequestResult()
{
    delete this->response;
}