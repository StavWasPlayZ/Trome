#pragma once

#include <optional>

struct ProtocolResponse;
class IRequestHandler;

struct RequestResult
{
    explicit RequestResult(const ProtocolResponse* response,
        const std::optional<const IRequestHandler*>& newHandler = std::nullopt);

    ~RequestResult();

    const ProtocolResponse* response;

    /**
     * NOTE: THIS RESOURCE MUST BE FREED
     */
    const std::optional<const IRequestHandler*> newHandler;
};
