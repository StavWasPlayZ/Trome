#include "Server.h"

std::future<void>& Server::run()
{
	return this->m_communicator.bindAndListen();
}

void Server::close()
{
	m_communicator.close();
}

bool Server::isRunning() const
{
	return this->m_communicator.isRunning();
}
