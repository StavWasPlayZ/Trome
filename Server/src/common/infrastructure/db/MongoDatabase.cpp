#include "MongoDatabase.h"

#include <iostream>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>

namespace bson_builder = bsoncxx::builder::basic;

const std::hash<std::string> MongoDatabase::ID_HASHER {};

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
    try
    {
        setupMongoConnection();
        setupDbConnections();
    }
    catch (const std::exception& e)
    {
        std::cout << "Error initiating MongoDB: " << e.what() << std::endl;
        return false;
    }

    std::cout << "Successfully connected to MongoDB" << std::endl;
    return true;
}

void MongoDatabase::setupMongoConnection()
{
    // Mostly copied from the official docs:

    const auto uri = mongocxx::uri {CONNECTION_STRING};

    // Set the version of the Stable API on the client
    mongocxx::options::client client_options;
    const auto api = mongocxx::options::server_api{mongocxx::options::server_api::version::k_version_1};
    client_options.server_api_opts(api);

    // Set up the connection and get a handle on the "admin" database.
    this->m_mongoClient = { uri, client_options };
    mongocxx::database adminDb = this->m_mongoClient["admin"];

    // Ping the database.
    const auto ping_cmd = bson_builder::make_document(
        bson_builder::kvp("ping", 1)
    );

    adminDb.run_command(ping_cmd.view());
}

void MongoDatabase::setupDbConnections()
{
    m_db = m_mongoClient.database("trome_db");
}

//TODO: Convert usage of numbers as IDs to the unique IDs format used in Mongo,
// in SQLite too.
// Then remove this method.
int MongoDatabase::objIdToNumericId(const bsoncxx::oid& id)
{
    return ID_HASHER(id.to_string());
}

mongocxx::collection MongoDatabase::usersCollection() const
{
    return this->m_db.collection("users");
}

bool MongoDatabase::close()
{
    // Done automatically on database destructor.
    return true;
}

bool MongoDatabase::doesUserExist(const std::string &username) const
{
    const auto userFilter = bson_builder::make_document(
        bson_builder::kvp("username", username)
    );

    return usersCollection().count_documents(userFilter.view()) != 0;
}

unsigned int MongoDatabase::queryIdOfUser(const std::string &username, const std::string &password) const
{
    const auto userFilter = bson_builder::make_document(
        bson_builder::kvp("username", username),
        bson_builder::kvp("password", password)
    );

    const auto result = usersCollection().find_one(userFilter.view());

    if (!result.has_value())
        return -1;

    return objIdToNumericId(result.value().view()["_id"].get_oid().value);
}

unsigned int MongoDatabase::addNewUser(const std::string &username, const std::string &password,
                                       const std::string &email, const std::string &phone,
                                       const std::string &birthdate,
                                       const std::optional<std::string> &address) const
{
    // Because Magshimim requires no double usernames
    if (doesUserExist(username))
        throw std::runtime_error("User already exists (UNIQUE)");

    bson_builder::document addUserBuilder {};

    addUserBuilder.append(
        bson_builder::kvp("username", username),
        bson_builder::kvp("password", password),
        bson_builder::kvp("email", email),
        bson_builder::kvp("phone", phone),
        bson_builder::kvp("birthdate", birthdate)
    );

    if (address.has_value())
    {
        addUserBuilder.append(bson_builder::kvp("address", address.value()));
    }

    const auto result = usersCollection().insert_one(addUserBuilder.extract());

    return objIdToNumericId(result.value().inserted_id().get_oid().value);
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

MongoDatabase::MongoDatabase()
{
}
