#pragma once

#include "infrastructure/Communicator.h"

class Server
{
public:
	/**
	* Runs the server.
	*
	* Returns: The future handling the client sockets.
	* Completes when server closes.
	*/
	std::future<void>& run();

	void close();

	bool isRunning() const;

private:
	Communicator m_communicator;
};
