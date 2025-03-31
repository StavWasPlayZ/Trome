#pragma once

// #include "infrastructure/CommonCommunicator.hpp"
#include "platform/windows/infrastructure/WindowsCommunicator.h"

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
	WindowsCommunicator m_communicator;
};
