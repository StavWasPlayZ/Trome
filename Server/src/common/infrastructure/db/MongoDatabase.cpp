#include "MongoDatabase.h"

#include <iostream>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>

//TODO: Save in app resources, then change credentials.
// https://cloud.mongodb.com/v2/68517c6ca848c6706d2cdf60#/security/database
const std::string MongoDatabase::CONNECTION_STRING = "mongodb+srv://cstav:XTXUNBCmGNbzkTLK@cluster0.xbibizy.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0";

MongoDatabase &MongoDatabase::getInstance()
{
    static MongoDatabase instance;
    return instance;
}

MongoDatabase::~MongoDatabase()
{
}

bool MongoDatabase::open()
{
    // Copied from the official docs:
    try
    {
        const auto uri = mongocxx::uri {CONNECTION_STRING};

        // Set the version of the Stable API on the client
        mongocxx::options::client client_options;
        const auto api = mongocxx::options::server_api{mongocxx::options::server_api::version::k_version_1};
        client_options.server_api_opts(api);

        // Set up the connection and get a handle on the "admin" database.
        this->m_mongoClient = { uri, client_options };
        mongocxx::database adminDb = this->m_mongoClient["admin"];

        // Ping the database.
        const auto ping_cmd = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("ping", 1));
        adminDb.run_command(ping_cmd.view());

        m_db = m_mongoClient.database("trome_db");
    }
    catch (const std::exception& e)
    {
        std::cout << "Error initiating MongoDB: " << e.what() << std::endl;
        return false;
    }

    std::cout << "Successfully connected to MongoDB" << std::endl;
    return true;
}

bool MongoDatabase::close()
{
    // Done automatically on database destructor.
    return true;
}

bool MongoDatabase::doesUserExist(const std::string &username) const
{
}

unsigned int MongoDatabase::queryIdOfUser(const std::string &username, const std::string &password) const
{
}

unsigned int MongoDatabase::addNewUser(const std::string &username, const std::string &password,
                                       const std::string &email, const std::string &phone,
                                       const std::string &birthdate,
                                       const std::optional<std::string> &address) const
{
}

int MongoDatabase::queryQuestionsCount() const
{
}

std::list<Question> MongoDatabase::queryQuestions(int amount) const
{
}

void MongoDatabase::addQuestions(std::vector<Question> questions,
                                 const std::optional<std::string> &authorName) const
{
}

void MongoDatabase::addToStats(const std::string &username, int time, int answers, int correctAnswers,
                               int points, int games) const
{
}

std::map<UserModel, int> MongoDatabase::queryHighScores(int limit) const
{
}

std::optional<UserStatistics> MongoDatabase::getUserStatisticsById(unsigned int id) const
{
}

unsigned int MongoDatabase::queryIdOfUser(const std::string &username) const
{
}

MongoDatabase::MongoDatabase()
{
}
