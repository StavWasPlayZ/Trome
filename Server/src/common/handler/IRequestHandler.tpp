#pragma once

#include "IRequestHandler.h"

#include "infrastructure/request/RequestResult.h"

#include "codec/s2c/Response.h"
#include "codec/s2c/JsonResponsePacketSerializer.h"


template <typename T>
RequestResult IRequestHandler::errorUnimplementedResult(const RequestCode reqId) const
{
    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(
            ErrorResponse(
                ErrorStatus::SERVER_UNIMPLEMENTED,
                reqId,
                "Unimplemented server-side"
            )
        ),
        new T(*static_cast<T *>(this))
    );
}
