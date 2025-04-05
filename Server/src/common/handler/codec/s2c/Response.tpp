#include "Response.h"

template <typename S>
inline ProtocolResponse<S>::ProtocolResponse(const S status) :
    status(status)
{}

template <typename S>
inline RegistrationResponse<S>::RegistrationResponse(const S status, const unsigned int userId) : ProtocolResponse<S>(status),
    userId(userId)
{}

template <typename S>
inline RegistrationResponse<S>::RegistrationResponse(const S status) : RegistrationResponse(status, -1)
{}
