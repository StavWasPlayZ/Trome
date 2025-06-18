#include <iostream>

#include "infrastructure/Server.h"

const std::string CMD_EXIT = "EXIT";

Server& server = Server::getInstance();

void cmdHandler();

int main()
{
    std::srand(std::time(nullptr));

	server.run();
	cmdHandler();
}

void cmdHandler()
{
    std::cout << "Server started. Type EXIT to close." << std::endl;

	while (server.isRunning())
	{
		std::string cmd;
		std::getline(std::cin, cmd);

		if (cmd == CMD_EXIT)
		{
			server.close();
			return;
		}
		else
		{
			std::cerr << "Unrecognized command" << std::endl;
		}
	}
}
