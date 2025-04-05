#include "Response.h"

template <typename S>
inline RegistrationResponse<S>::RegistrationResponse(const S status, const unsigned int userId) :
    status(status),
    userId(userId)
{}

template <typename S>
RegistrationResponse<S>::RegistrationResponse(const S status) : RegistrationResponse(status, -1)
{}
