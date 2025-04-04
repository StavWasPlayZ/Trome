#pragma once

#include "IRequestHandler.h"

#include "infrastructure/request/RequestResult.h"

#include "codec/s2c/Response.h"
#include "codec/s2c/JsonResponsePacketSerializer.h"


template <typename T>
RequestResult IRequestHandler::errorServerUnimplemented() const
{
    return RequestResult(
        JsonResponsePacketSerializer::serializeResponse(ErrorResponse("Unimplemented server-side")),
        new T(*((T*)this))
    );
}
