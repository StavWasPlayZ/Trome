#include "Client.h"

#include "Server.h"

Client::Client(const SOCKET socket, const IRequestHandler *const requestHandler) :
    socket(socket),
    requestHandler(requestHandler),
    thread(nullptr)
{}

Client::~Client()
{
    delete this->requestHandler;
    delete this->thread;
}

const IRequestHandler *Client::getRequestHandler() const
{
    return this->requestHandler;
}

void Client::setRequestHandler(const IRequestHandler *const requestHandler)
{
    this->requestHandler = requestHandler;
}

void Client::lockRequestHandler()
{
    this->requestHandlerMutex.lock();
}

void Client::releaseRequestHandler()
{
    this->requestHandlerMutex.unlock();
}

const std::future<void> &Client::getThread() const
{
    return *this->thread;
}

void Client::setAndStartThread(const std::function<void()> &threadFunc)
{
    if (this->thread != nullptr)
    {
        throw std::runtime_error("Client thread already set");
    }

    this->thread = new std::future(std::async(std::launch::async, threadFunc));
}


void Client::handleDisconnecting() const
{
    Server::getInstance().getLoginManager().logout(*this);
}
