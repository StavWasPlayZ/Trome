#include "Server.h"

#include <iostream>
#include <exception>

#pragma comment(lib, "ws2_32.lib")

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
        throw std::exception("WSAStartup failed");
    }

    // Create socket file descriptor
    if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        close();
        throw std::exception("Socket creation failed");
    }

    // Bind the socket to the port
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(PORT);

    if (bind(this->_serverSocket, (struct sockaddr*)&_address, sizeof(_address)) == SOCKET_ERROR)
{
        close();
        throw std::exception("Bind failed");
    }

    // Start listening for connections
    if (listen(this->_serverSocket, 3) == SOCKET_ERROR)
{
        close();
        throw std::exception("Listen failed");
    }

    std::cout << "Listening on port " << PORT << "..." << std::endl;

    // Accept incoming connections
    _acceptClients();

    std::cout << "Connection accepted" << std::endl;
    close();
}

void Server::_acceptClients() const
{
    int addrLen = sizeof(this->_address);

    //while (true)
    {
        SOCKET new_socket;

        if ((new_socket = accept(this->_serverSocket, (struct sockaddr*)&_address, &addrLen)) == INVALID_SOCKET)
        {
            throw std::exception("Accept failed");
            closesocket(this->_serverSocket);
            WSACleanup();
        }
    }
}
