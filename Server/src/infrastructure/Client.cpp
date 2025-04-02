#include "Client.h"

Client::Client(const SOCKET socket, IRequestHandler* const requestHandler, const std::function<void()> clientThreadFunc) :
	socket(socket),
	requestHandler(requestHandler)
{
	thread = std::async(std::launch::async, clientThreadFunc);
}

Client::~Client()
{
	delete this->requestHandler;
}

std::future<void>& Client::getThread()
{
	return this->thread;
}
