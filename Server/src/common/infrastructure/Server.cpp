#include "Server.h"

#include "db/SqliteDatabase.h"

Server::Server() :
    m_database(SqliteDatabase::getInstance()),
    m_loginManager(m_database),
    m_statisticsManager(m_database),
    m_handlerFactory(this->m_loginManager, this->m_roomManager, this->m_statisticsManager, this->m_database),
    m_communicator(Communicator::getInstance(this->m_handlerFactory))
{}

Server &Server::getInstance()
{
    static Server instance;
    return instance;
}

void Server::run() const
{
	m_database.open();
	this->m_communicator.bindAndListen();
}

void Server::close() const
{
	this->m_communicator.close();
}

bool Server::isRunning() const
{
	return this->m_communicator.isRunning();
}
