#include "LinuxCommunicator.h"

#include "handler/LoginRequestHandler.h"

#include <iostream>

LinuxCommunicator::LinuxCommunicator() :
    m_serverSocket(0)
{}

std::future<void> &LinuxCommunicator::bindAndListen()
{
    return startServerThread();
}

void LinuxCommunicator::platformClose()
{
    
}

void LinuxCommunicator::acceptClients()
{
    
}

void LinuxCommunicator::closeClientSocket(const int socket)
{
    
}

bool LinuxCommunicator::recieveMsg(const int socket, char* buffer, const int length) const
{
    return true;
}

void LinuxCommunicator::sendMsg(const int socket, const char* buffer, const int length) const
{
    
}
