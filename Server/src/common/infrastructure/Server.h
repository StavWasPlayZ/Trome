#pragma once

#include "Communicator.h"
#include "manager/LoginManager.h"
#include "handler/RequestHandlerFactory.h"
#include "db/IDatabase.h"

class Server
{
public:
    // https://stackoverflow.com/a/1008289
    Server(const Server&) = delete;
    void operator=(const Server&) = delete;

    static Server& getInstance();

	/**
	* Runs the server.
	*/
	void run() const;

	void close() const;

	bool isRunning() const;

private:
    Server();

    /**
    * Populates the Questions table of the DB with
    * questions from opentdb.com
    */
    void populateQuestionsFromWeb(int amount = 100) const;
    static std::vector<Question> deserializeWebQuestions(const nlohmann::json& data);

	IDatabase& m_database;

	LoginManager m_loginManager;
    RoomManager m_roomManager;
    StatisticsManager m_statisticsManager;
	
	RequestHandlerFactory m_handlerFactory;
	Communicator& m_communicator;
};
