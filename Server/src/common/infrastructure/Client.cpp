#include "Client.h"

Client::Client(const SOCKET socket, const IRequestHandler *const requestHandler, const std::function<void()> &clientThreadFunc) :
    socket(socket),
    requestHandler(requestHandler),
    thread(std::async(std::launch::async, clientThreadFunc))
{}

Client::~Client()
{
    delete this->requestHandler;
}
