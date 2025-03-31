#include "ForcedDisconnectionException.h"

const char *ForcedDisconnectionException::what() const noexcept
{
    return "Client has forcibly disconnected";
}
