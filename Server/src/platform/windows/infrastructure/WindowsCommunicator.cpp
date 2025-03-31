#include "WindowsCommunicator.h"

#include "exception/WSAException.h"

#include "handler/LoginRequestHandler.h"

#include <iostream>

WindowsCommunicator::WindowsCommunicator() :
    m_serverSocket(INVALID_SOCKET),
    _address({ 0 })
{}

std::future<void> &WindowsCommunicator::bindAndListen()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        throw WSAException("WSAStartup failed");
    }

    // Create socket file descriptor
    this->m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->m_serverSocket == INVALID_SOCKET)
    {
        close();
        throw WSAException("Socket creation failed");
    }

    // The timeout for the recv method
    // Set in place to allow refreshing the value of _running.
    setsockopt(this->m_serverSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&RECV_REFRESH_TIMEOUT, sizeof(RECV_REFRESH_TIMEOUT));

    // Bind the socket to the port
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(PORT);

    if (bind(this->m_serverSocket, (struct sockaddr*)&_address, sizeof(_address)) == SOCKET_ERROR)
    {
        close();
        throw WSAException("Bind failed");
    }

    this->_running = true;

    // Start listening for connections
    if (listen(this->m_serverSocket, 3) == SOCKET_ERROR)
    {
        close();
        throw WSAException("Listen failed");
    }

    return startServerThread();
}

void WindowsCommunicator::platformClose()
{
    if (this->m_serverSocket != INVALID_SOCKET)
    {
        closesocket(this->m_serverSocket);
    }

    WSACleanup();

    this->m_serverSocket = INVALID_SOCKET;
    this->_address = { 0 };

    this->_serverThread = std::future<void>();
}

void WindowsCommunicator::acceptClients()
{
    int addrLen = sizeof(this->_address);
    const SOCKET newSocket = accept(this->m_serverSocket, (struct sockaddr*)&_address, &addrLen);

    if (newSocket == INVALID_SOCKET)
    {
        throw WSAException("Accept failed");
    }

    registerClient(newSocket);
}

void WindowsCommunicator::closeClientSocket(const SOCKET socket)
{
    closesocket(socket);
}

bool WindowsCommunicator::recieveMsg(const SOCKET socket, char* buffer, const int length) const
{
    int result = recv(socket, buffer, length, 0);

    if (result == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAETIMEDOUT)
        {
            return false;
        }

        throw WSAException("Error occured while handling client socket " + std::to_string(socket));
    }

    return true;
}

void WindowsCommunicator::sendMsg(const SOCKET socket, const char* buffer, const int length) const
{
    int sent = send(socket, buffer, length, 0);

    if (sent == -1)
    {
        throw WSAException("Failed to send message to client socket " + std::to_string(socket));
    }
}
