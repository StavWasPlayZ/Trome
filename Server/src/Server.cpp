#include "Server.h"

#include "exception/WSAException.h"

#include <iostream>

const std::string Server::CMD_EXIT = "EXIT";
const std::string Server::CMD_HELLO = "Hello";

Server::Server() :
    _serverSocket(INVALID_SOCKET),
    _address({0})
{}

Server::~Server()
{
    close();
}

void Server::close()
{
    if (this->_serverSocket != INVALID_SOCKET)
    {
        closesocket(this->_serverSocket);
    }

    WSACleanup();

    this->_serverSocket = INVALID_SOCKET;
    this->_address = { 0 };
}

void Server::run()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        throw WSAException("WSAStartup failed");
    }

    // Create socket file descriptor
    if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        close();
        throw WSAException("Socket creation failed");
    }

    // Bind the socket to the port
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(PORT);

    if (bind(this->_serverSocket, (struct sockaddr*)&_address, sizeof(_address)) == SOCKET_ERROR)
{
        close();
        throw WSAException("Bind failed");
    }

    // Start listening for connections
    if (listen(this->_serverSocket, 3) == SOCKET_ERROR)
{
        close();
        throw WSAException("Listen failed");
    }

    std::cout << "Listening on port " << PORT << "..." << std::endl;

    // Accept incoming connections
    _acceptClients();

    close();
}

void Server::_acceptClients() const
{
    int addrLen = sizeof(this->_address);

    while (true)
    {
        SOCKET newSocket;

        if ((newSocket = accept(this->_serverSocket, (struct sockaddr*)&_address, &addrLen)) == INVALID_SOCKET)
        {
            throw WSAException("Accept failed");
        }

        std::cout << "Connection accepted" << std::endl;

        closesocket(newSocket);
    }
}
