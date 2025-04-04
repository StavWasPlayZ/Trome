#include "Server.h"

#include "db/SqliteDatabase.h"

Server::Server() :
	m_database(new SqliteDatabase()),
	m_communicator(this->m_handlerFactory),
	m_handlerFactory(this->m_loginManager, this->m_database)
{}

Server::~Server()
{
	delete this->m_database;
}

void Server::run()
{
	this->m_communicator.bindAndListen();
}

void Server::close()
{
	this->m_communicator.close();
}

bool Server::isRunning() const
{
	return this->m_communicator.isRunning();
}
