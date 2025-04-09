#include "UnixCommunicator.h"

#include "handler/LoginRequestHandler.h"

#include <iostream>

#include "exception/SocketDisconnectionException.h"
#include "exception/SocketTimeoutException.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>

UnixCommunicator::UnixCommunicator(const RequestHandlerFactory& handlerFactory) : CommonCommunicator(0, handlerFactory)
{}

bool UnixCommunicator::isValidSocket(const int result) const
{
    return result != 0;
}

bool UnixCommunicator::isValidBind(const int result) const
{
    return result >= 0;
}

bool UnixCommunicator::isValidListen(const int result) const
{
    return result >= 0;
}

void UnixCommunicator::platformClose()
{
    if (this->m_serverSocket != 0)
    {
        ::close(this->m_serverSocket);
    }

    this->m_serverSocket = 0;
}

void UnixCommunicator::closeClientSocket(const int socket)
{
    if (socket == 0)
    {
        return;
    }

    ::close(socket);
}

void UnixCommunicator::acceptClients()
{
    socklen_t addrLen = sizeof(this->_serverSockAddr);
    //NOTE: When closing with closesocket in Windows, this immediately terminates the accept operation.
    // It is not the same under Unix.
    const int newSocket = accept(this->m_serverSocket, (struct sockaddr*)&_serverSockAddr, &addrLen);

    if (newSocket < 0)
    {
        // Additionally, the timeouts applied to the recv method also apply to the accept above.
        // Thus, we'll simply ignore all timeout failures.
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return;
        }

        throwPlatformError("Accept failed");
        throw std::exception();
    }

    registerClient(newSocket);
}

void UnixCommunicator::throwPlatformError(const std::string &msg) const
{
    throw std::runtime_error(msg + ": " + std::to_string(errno));
}

void UnixCommunicator::setRecvTimeout(const unsigned int timeoutMs) const
{
    struct timeval timeoutVal;
    timeoutVal.tv_sec = timeoutMs / 1000;
    timeoutVal.tv_usec = (timeoutMs % 1000) * 1000;

    setsockopt(
        this->m_serverSocket,
        SOL_SOCKET,
        SO_RCVTIMEO,
        &timeoutVal,
        sizeof(timeoutVal)
    );

    return;
}

void UnixCommunicator::recieveMsg(const int socket, void *buffer, const int length) const
{
    const ssize_t result = recv(socket, buffer, length, 0);

    if (result == 0)
    {
        // Client has ✨✨gracefully✨✨ disconnected
        // Still throw an error to catch this event
        throw SocketDisconnectionException();
    }

    if (result == -1)
    {
        // Supposedly, this is timeout.
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            throw SocketTimeoutException();
        }

        if (errno == ECONNRESET || errno == EPIPE)
        {
            throw SocketDisconnectionException();
        }

        throwPlatformError("Error occured while handling client socket " + std::to_string(socket));
        throw std::exception();
    }
}
