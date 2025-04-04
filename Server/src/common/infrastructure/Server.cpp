#include "Server.h"

void Server::run()
{
	this->m_communicator.bindAndListen();
}

void Server::close()
{
	m_communicator.close();
}

bool Server::isRunning() const
{
	return this->m_communicator.isRunning();
}
