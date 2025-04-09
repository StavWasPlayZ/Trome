#include "SocketDisconnectionException.h"

const char *SocketDisconnectionException::what() const noexcept
{
    return "Client has disconnected";
}
