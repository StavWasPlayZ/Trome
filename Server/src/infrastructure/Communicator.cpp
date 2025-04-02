#include "Communicator.h"

#include "../exception/WSAException.h"

#include "../handler/LoginRequestHandler.h"

#include <iostream>

const std::string Communicator::CMD_HELLO = "Hello";

Communicator::Communicator() :
    m_serverSocket(INVALID_SOCKET),
    _address({ 0 }),
    _running(false)
{}

Communicator::~Communicator()
{
    close();
}

void Communicator::close()
{
    if (!this->_running)
        return;

    // Notify all threads that the server is closing
    this->_running = false;

    // Wait for 'em to close
    this->m_clients_mutex.lock();
    for (const auto& client : this->m_clients)
    {
        client.second->getThread().wait();
    }
    this->m_clients_mutex.unlock();


    if (this->m_serverSocket != INVALID_SOCKET)
    {
        closesocket(this->m_serverSocket);
    }

    WSACleanup();


    this->m_serverSocket = INVALID_SOCKET;
    this->_address = { 0 };

    this->_serverThread = std::future<void>();
}

bool Communicator::isRunning() const
{
    return this->_running;
}

std::future<void>& Communicator::bindAndListen()
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

    std::cout << "Listening on port " << PORT << "..." << std::endl;

    // Start the server thread
    return this->_serverThread = std::async(
        std::launch::async,
        [this]()
        {
            _serverThreadFunc();
        }
    );
}

void Communicator::_serverThreadFunc()
{
    while (this->_running)
    {
        _acceptClients();
        _freeDisconnectedClients();
    }

    close();
}

void Communicator::_freeDisconnectedClients()
{
    this->_disconnectingClients_mutex.lock();
    this->m_clients_mutex.lock();

    for (const auto& clientSock : this->_disconnectingClients)
    {
        delete this->m_clients.at(clientSock);
        m_clients.erase(clientSock);
    }

    this->m_clients_mutex.unlock();
    this->_disconnectingClients_mutex.unlock();
}

void Communicator::_acceptClients()
{
    int addrLen = sizeof(this->_address);
    const SOCKET newSocket = accept(this->m_serverSocket, (struct sockaddr*)&_address, &addrLen);

    if (newSocket == INVALID_SOCKET)
    {
        throw WSAException("Accept failed");
    }

    std::cout << "Connection accepted" << std::endl;

    this->m_clients[newSocket] = new Client(
        newSocket,
        new LoginRequestHandler(),
        [this, newSocket]()
        {
            _clientThreadFunc(newSocket);
        }
    );
}

void Communicator::_clientThreadFunc(const SOCKET socket)
{
    sendMsg(socket, CMD_HELLO.c_str(), CMD_HELLO.length(), 0);

    while (this->_running)
    {
        char buffer[6];
        if (!recieveMsg(socket, buffer, sizeof(buffer), 0))
        {
            // If we timed out (see RECV_REFRESH_TIMEOUT),
            // simply wait for the next recv cycle (if applicable).
            continue;
        }

        buffer[5] = 0;

        if (buffer == CMD_HELLO)
        {
            sendMsg(socket, CMD_HELLO.c_str(), CMD_HELLO.length(), 0);
        }
    }

    _disconnectClient(socket);
}

void Communicator::_disconnectClient(const SOCKET socket)
{
    closesocket(socket);

    this->_disconnectingClients_mutex.lock();
    this->_disconnectingClients.push_back(socket);
    this->_disconnectingClients_mutex.unlock();
}

bool Communicator::recieveMsg(const SOCKET socket, char* buffer, const int length, const int flags) const
{
    int result = recv(socket, buffer, length, flags);

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

void Communicator::sendMsg(const SOCKET socket, const char* buffer, const int length, const int flags) const
{
    int sent = send(socket, buffer, length, flags);

    if (sent == -1)
    {
        throw WSAException("Failed to send message to client socket " + std::to_string(socket));
    }
}
