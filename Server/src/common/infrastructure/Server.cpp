#include "Server.h"

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "db/SqliteDatabase.h"
#include "infrastructure/Question.h"

Server::Server() :
    m_database(SqliteDatabase::getInstance()),
    m_loginManager(m_database),
    m_statisticsManager(m_database),
    m_handlerFactory(this->m_loginManager, this->m_roomManager, this->m_statisticsManager, this->m_database),
    m_communicator(Communicator::getInstance(this->m_handlerFactory))
{}

void Server::populateQuestionsFromWeb(const int amount) const
{
    std::cout << "Populating DB with web questions..." << std::endl;

    if (m_database.queryQuestionsCount() > 0)
    {
        std::cout << "DB already populated; Aborting operation" << std::endl;
        return;
    }

    std::cout << "Fetching questions from web..." << std::endl;

    httplib::Client client("https://opentdb.com");
    const httplib::Result response = client.Get("/api.php?"
        "category=18"
        "&type=multiple"
        "&amount=" + std::to_string(amount)
    );

    const nlohmann::json parsedResponse = nlohmann::json::parse(response->body);

    const int resStatus = parsedResponse.at("response_code");

    if (resStatus != 0)
    {
        std::cerr << "Fetching unsuccessful with status " << resStatus << std::endl;
        return;
    }

    std::cout << "Fetching successful with status " << resStatus << std::endl;

    m_database.addQuestions(deserializeWebQuestions(parsedResponse), std::nullopt);

    std::cout << "Successfully populated web questions" << std::endl;
}

std::vector<Question> Server::deserializeWebQuestions(const nlohmann::json &data)
{
    std::vector<Question> questions;

    for (const nlohmann::json &questionRaw : data.at("results"))
    {
        std::vector<std::string> answers;
        answers.reserve(4);

        answers.push_back(questionRaw.at("correct_answer"));

        for (const nlohmann::json &answerRaw : questionRaw.at("incorrect_answers"))
        {
            answers.push_back(answerRaw.get<std::string>());
        }

        const Question question(questionRaw.at("question"), answers);

        questions.push_back(question);
    }

    return questions;
}

Server &Server::getInstance()
{
    static Server instance;
    return instance;
}

void Server::run() const
{
	m_database.open();
    populateQuestionsFromWeb();

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
