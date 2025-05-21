#include "ErrorResponse.h"

ErrorResponse::ErrorResponse(const ErrorStatus status, const RequestCode reqCode,
                             const std::optional<std::string>& context) :
    ProtocolResponse(ResponseCode::ERROR),
    reqCode(reqCode),
    status(status),
    context(context)
{}