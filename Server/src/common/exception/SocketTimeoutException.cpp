#include "SocketTimeoutException.h"

const char *SocketTimeoutException::what() const noexcept
{
    return "Client socket has timed out";
}
