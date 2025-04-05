#include "Response.h"

LoginResponse::LoginResponse(const LoginStatus status, const unsigned int userId) : RegistrationResponse(status, userId)
{}

LoginResponse::LoginResponse(const LoginStatus status) : RegistrationResponse(status)
{}

SignupResponse::SignupResponse(const SignupStatus status, const unsigned int userId) : RegistrationResponse(status, userId)
{}

SignupResponse::SignupResponse(const SignupStatus status) : SignupResponse(status, -1)
{}

ErrorResponse::ErrorResponse(const std::string &message) :
    message(message)
{}
