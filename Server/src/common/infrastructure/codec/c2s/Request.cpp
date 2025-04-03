#include "Request.h"

LoginRequest::LoginRequest(const std::string &username, const std::string &password) :
    username(username),
    password(password)
{}

SignupRequest::SignupRequest(const std::string &username, const std::string &password, const std::string &email) :
    username(username),
    password(password),
    email(email)
{}
