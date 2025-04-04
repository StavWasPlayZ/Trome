#pragma once

#include "Communicator.h"
#include "LoginManager.h"
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
	LoginManager m_loginManager;
	IDatabase* const m_database;
	
	Communicator m_communicator;
	RequestHandlerFactory m_handlerFactory;
};
