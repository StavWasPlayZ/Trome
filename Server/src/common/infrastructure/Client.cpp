#include "Client.h"

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
