#pragma once

#include "Communicator.h"

class Server
{
public:
	/**
	* Runs the server.
	*/
	void run();

	void close();

	bool isRunning() const;

private:
	Communicator m_communicator;
};
