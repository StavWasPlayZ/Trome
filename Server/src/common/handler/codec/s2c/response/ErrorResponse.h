#pragma once

#include "../../c2s/request/Request.h"
#include "Response.h"

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
    UNKNOWN_RESOURCE,

    // Login
    INVALID_CREDENTIALS,
    ALREADY_LOGGED_IN,

    // Signup
    USERNAME_TAKEN,
    INVALID_ARGUMENT,

    // Logout
    NOT_LOGGED_IN,

    // Room
    ROOM_FULL,
    ROOM_ALREADY_PLAYING,

    // Game
    QUESTION_OUTDATED,
    ALREADY_FINISHED
};
