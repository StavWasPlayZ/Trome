#pragma once

#include "Response.h"

template <typename S>
ProtocolResponse<S>::ProtocolResponse(const S status) :
    status(status)
{}

template <typename S>
RegistrationResponse<S>::RegistrationResponse(const S status, const unsigned int userId) : ProtocolResponse<S>(status),
    userId(userId)
{}

template <typename S>
RegistrationResponse<S>::RegistrationResponse(const S status) : RegistrationResponse(status, -1)
{}
