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
	Communicator m_communicator;
    LoginManager m_loginManager;
	IDatabase* const m_database;

	RequestHandlerFactory m_handlerFactory;
};
