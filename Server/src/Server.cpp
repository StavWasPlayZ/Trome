#include "Server.h"

#include "exception/WSAException.h"

#include <iostream>

const std::string Server::CMD_HELLO = "Hello";

Server::Server() :
    _serverSocket(INVALID_SOCKET),
    _address({0}),
    _running(false)
{}

Server::~Server()
{
    close();
}

void Server::close()
{
    if (!this->_running)
        return;

    // Notify all threads that the server is closing
    this->_running = false;

    // Wait for 'em to close
    for (std::future<void>& future : this->_clientThreads)
    {
        future.wait();
    }

    if (this->_serverSocket != INVALID_SOCKET)
    {
        closesocket(this->_serverSocket);
    }

    WSACleanup();

    this->_serverSocket = INVALID_SOCKET;
    this->_address = { 0 };

    this->_serverThread = std::future<void>();
}

bool Server::isRunning() const
{
    return this->_running;
}

std::future<void>& Server::run()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        throw WSAException("WSAStartup failed");
    }

    // Create socket file descriptor
    this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_serverSocket == INVALID_SOCKET)
    {
        close();
        throw WSAException("Socket creation failed");
    }

    // The timeout for the recv method.
    // Set in place to allow refreshing the value of _running.
    setsockopt(this->_serverSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&RECV_REFRESH_TIMEOUT, sizeof(RECV_REFRESH_TIMEOUT));

    // Bind the socket to the port
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(PORT);

    if (bind(this->_serverSocket, (struct sockaddr*)&_address, sizeof(_address)) == SOCKET_ERROR)
    {
        close();
        throw WSAException("Bind failed");
    }

    this->_running = true;

    // Start listening for connections
    if (listen(this->_serverSocket, 3) == SOCKET_ERROR)
{
        close();
        throw WSAException("Listen failed");
    }

    std::cout << "Listening on port " << PORT << "..." << std::endl;

    return this->_serverThread = std::async(
        std::launch::async,
        [this]()
        {
            _acceptClients();
        }
    );
}

void Server::_acceptClients()
{
    int addrLen = sizeof(this->_address);

    while (this->_running)
    {
        const SOCKET newSocket = accept(this->_serverSocket, (struct sockaddr*)&_address, &addrLen);

        if (newSocket == INVALID_SOCKET)
        {
            throw WSAException("Accept failed");
        }

        std::cout << "Connection accepted" << std::endl;

        this->_clientThreads.push_back(std::async(
            std::launch::async,
            [this, newSocket]()
            {
                _handleClient(newSocket);
            }
        ));
    }

    close();
}

void Server::_handleClient(const SOCKET socket) const
{
    while (this->_running)
    {
        char buffer[5];
        int bytesReceived = recv(socket, buffer, sizeof(buffer), 0);

        std::cout << "dfasjhgb" << std::endl;
    }

    closesocket(socket);
}
