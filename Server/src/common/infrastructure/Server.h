#pragma once

#include "Communicator.h"
#include "manager/LoginManager.h"
#include "handler/RequestHandlerFactory.h"
#include "db/IDatabase.h"

class Server
{
public:
	Server();
	~Server();

	/**
	* Runs the server.
	*/
	void run();

	void close();

	bool isRunning() const;

private:
	IDatabase* const m_database;
	LoginManager m_loginManager;
	
	RequestHandlerFactory m_handlerFactory;
	Communicator m_communicator;
};
