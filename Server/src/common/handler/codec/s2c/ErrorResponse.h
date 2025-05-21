#pragma once

#include "Response.h"
#include "handler/codec/c2s/Request.h"

enum class ErrorStatus : unsigned int;


struct ErrorResponse : ProtocolResponse
{
    ErrorResponse(ErrorStatus status, RequestCode reqCode, const std::optional<std::string>& context = std::nullopt);

    /**
     * The original request code
     */
    const RequestCode reqCode;

    const ErrorStatus status;
    const std::optional<std::string> context;
};


enum class ErrorStatus : unsigned int
{
    // Generic
    INTERNAL,
    SERVER_UNIMPLEMENTED,
    ILLEGAL_REQUEST,

    // The status of a response that takes in a resource.
    //
    // May be of any type.
    //
    // E.g: User not found.
    ERROR_UNKNOWN_RESOURCE,

    // Login
    FAILED_INVALID_CREDENTIALS,
    FAILED_ALREADY_LOGGED_IN,

    // Signup
    FAILED_USERNAME_TAKEN,
    FAILED_INVALID_ARGUMENT,

    // Logout
    FAILED_NOT_LOGGED_IN
};
