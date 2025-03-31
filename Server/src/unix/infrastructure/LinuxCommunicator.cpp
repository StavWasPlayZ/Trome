#include "LinuxCommunicator.h"

#include "handler/LoginRequestHandler.h"

#include <iostream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>

LinuxCommunicator::LinuxCommunicator() :
    Communicator(0)
{}

bool LinuxCommunicator::isValidSocket(const int result) const
{
    return result != 0;
}

bool LinuxCommunicator::isValidBind(const int result) const
{
    return result >= 0;
}

bool LinuxCommunicator::isValidListen(const int result) const
{
    return result >= 0;
}

void LinuxCommunicator::platformClose()
{
    ::close(this->m_serverSocket);
    this->m_serverSocket = 0;
}

void LinuxCommunicator::closeClientSocket(const int socket)
{
    ::close(socket);
}

void LinuxCommunicator::acceptClients()
{
    socklen_t addrLen = sizeof(this->_serverSockAddr);
    const int newSocket = accept(this->m_serverSocket, (struct sockaddr*)&_serverSockAddr, &addrLen);

    if (newSocket < 0)
    {
        throwPlatformError("Accept failed");
        return;
    }

    registerClient(newSocket);
}

void LinuxCommunicator::throwPlatformError(const std::string &msg) const
{
    throw std::runtime_error(msg + ": " + std::to_string(errno));
}

void LinuxCommunicator::setRecvTimeout(const unsigned int timeoutMs) const
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

bool LinuxCommunicator::recieveMsg(const int socket, char *buffer, const int length) const
{
    ssize_t result = recv(socket, buffer, length, 0);

    if (result == -1)
    {
        // Supposedly, this is timeout.
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return false;
        }

        throw std::runtime_error("Error occured while handling client socket " + std::to_string(socket));
    }

    return true;
}
