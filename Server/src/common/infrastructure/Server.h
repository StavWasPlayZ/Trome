#pragma once

#include "PlatformCommunicator.h"

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
	PlatformCommunicator m_communicator;
};
