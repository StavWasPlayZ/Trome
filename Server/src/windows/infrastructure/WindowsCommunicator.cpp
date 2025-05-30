#include "WindowsCommunicator.h"

#include "windows/exception/WSAException.h"
#include "exception/SocketDisconnectionException.h"
#include "exception/SocketTimeoutException.h"

#include <iostream>

WindowsCommunicator::WindowsCommunicator(const RequestHandlerFactory *handlerFactory) :
    CommonCommunicator(INVALID_SOCKET, handlerFactory)
{}

WindowsCommunicator &WindowsCommunicator::getAndInitiateInstance(const RequestHandlerFactory *const handlerFactory)
{
    static WindowsCommunicator instance(handlerFactory);
    return instance;
}
WindowsCommunicator &WindowsCommunicator::getInstance()
{
    return getAndInitiateInstance(nullptr);
}

void WindowsCommunicator::bindAndListen()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        throwPlatformError("WSAStartup failed");
        throw std::exception();
    }

    CommonCommunicator::bindAndListen();
}

bool WindowsCommunicator::isValidSocket(const SOCKET result) const
{
    return result != INVALID_SOCKET;
}

bool WindowsCommunicator::isValidBind(const SOCKET result) const
{
    return result != SOCKET_ERROR;
}

bool WindowsCommunicator::isValidListen(const SOCKET result) const
{
    return result != SOCKET_ERROR;
}

void WindowsCommunicator::acceptClients()
{
    int addrLen = sizeof(this->_serverSockAddr);
    //NOTE: When closing with closesocket in Windows, this immediately terminates the accept operation.
    // It is not the same under Unix.
    const SOCKET newSocket = accept(this->m_serverSocket, (struct sockaddr*)&_serverSockAddr, &addrLen);

    if (newSocket == INVALID_SOCKET)
    {
        throwPlatformError("Accept failed");
        throw std::exception();
    }

    registerClient(newSocket);
}

void WindowsCommunicator::platformClose()
{
    if (this->m_serverSocket != INVALID_SOCKET)
    {
        closesocket(this->m_serverSocket);
    }

    WSACleanup();

    this->m_serverSocket = INVALID_SOCKET;
}

void WindowsCommunicator::closeClientSocket(const SOCKET socket)
{
    closesocket(socket);
}

void WindowsCommunicator::throwPlatformError(const std::string &msg) const
{
    throw WSAException(msg);
}

void WindowsCommunicator::setRecvTimeout(const unsigned int timeoutMs) const
{
    setsockopt(
        this->m_serverSocket,
        SOL_SOCKET,
        SO_RCVTIMEO,
        (const char*)&RECV_REFRESH_TIMEOUT_MS,
        sizeof(RECV_REFRESH_TIMEOUT_MS)
    );
}

void WindowsCommunicator::receiveMsg(const SOCKET socket, void *buffer, const int length) const
{
    const int result = recv(socket, (char*)buffer, length, 0);

    if (result == 0)
    {
        // Client has ✨✨gracefully✨✨ disconnected
        // Still throw an error to catch this event
        throw SocketDisconnectionException();
    }

    if (result == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAETIMEDOUT)
        {
            throw SocketTimeoutException();
        }

        if (WSAGetLastError() == WSAECONNRESET)
        {
            throw SocketDisconnectionException();
        }

        throwPlatformError("Error occured while handling client socket " + std::to_string(socket));
    }
}
