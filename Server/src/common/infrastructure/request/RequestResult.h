#pragma once

struct ProtocolResponse;
class IRequestHandler;

struct RequestResult
{
    RequestResult(const ProtocolResponse* response, const IRequestHandler* newHandler);
    ~RequestResult();

    const ProtocolResponse* response;

    /**
     * NOTE: THIS RESOURCE MUST BE FREED
     */
    const IRequestHandler* const newHandler;
};
