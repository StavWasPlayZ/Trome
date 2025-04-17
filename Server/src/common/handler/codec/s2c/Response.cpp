#include "Response.h"

LoginResponse::LoginResponse(const LoginStatus status, const unsigned int userId) : RegistrationResponse(status, userId)
{}

LoginResponse::LoginResponse(const LoginStatus status) : RegistrationResponse(status)
{}

SignupResponse::SignupResponse(const SignupStatus status, const unsigned int userId) : RegistrationResponse(status, userId)
{}

SignupResponse::SignupResponse(const SignupStatus status) : RegistrationResponse(status)
{}

LogoutResponse::LogoutResponse(const LogoutStatus status) : ProtocolResponse(status)
{}

ErrorResponse::ErrorResponse(const ErrorStatus status, const std::string &message) : ProtocolResponse(status),
    message(message)
{}
