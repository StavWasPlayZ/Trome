#include "SocketDisconnectionException.h"

SocketDisconnectionException::SocketDisconnectionException() = default;

const char *SocketDisconnectionException::what() const noexcept
{
    return "Client has disconnected";
}
