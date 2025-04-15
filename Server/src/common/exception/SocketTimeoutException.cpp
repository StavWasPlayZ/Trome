#include "SocketTimeoutException.h"

SocketTimeoutException::SocketTimeoutException() = default;

const char *SocketTimeoutException::what() const noexcept
{
    return "Client socket has timed out";
}
