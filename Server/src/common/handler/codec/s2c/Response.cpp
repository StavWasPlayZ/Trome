#include "Response.h"

LoginResponse::LoginResponse(const LoginResponse::Status status, const unsigned int userId) :
    status(status),
    userId(userId)
{}

LoginResponse::LoginResponse(const LoginResponse::Status status) : LoginResponse(status, -1)
{}

SignupResponse::SignupResponse(const SignupResponse::Status status, const unsigned int userId) :
    status(status),
    userId(userId)
{}

SignupResponse::SignupResponse(const SignupResponse::Status status) : SignupResponse(status, -1)
{}

ErrorResponse::ErrorResponse(const std::string &message) :
    message(message)
{}
