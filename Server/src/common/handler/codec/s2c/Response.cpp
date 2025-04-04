#include "Response.h"

LoginResponse::LoginResponse(const unsigned int status) :
    status(status)
{}

SignupResponse::SignupResponse(const unsigned int status) : 
    status(status)
{}

ErrorResponse::ErrorResponse(const std::string &message) :
    message(message)
{}
