#pragma once

#include "Response.h"

template <typename S>
ProtocolResponse<S>::ProtocolResponse(const ResponseCode id, const S status) :
    id(id),
    status(status)
{}

template <typename S>
RegistrationResponse<S>::RegistrationResponse(const ResponseCode id, const S status, const unsigned int userId) :
    ProtocolResponse<S>(id, status),
    userId(userId)
{}

template <typename S>
RegistrationResponse<S>::RegistrationResponse(const ResponseCode id, const S status) :
    RegistrationResponse(id, status, -1)
{}
