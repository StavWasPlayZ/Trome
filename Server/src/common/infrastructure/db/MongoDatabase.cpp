#include "MongoDatabase.h"

#include <iostream>
#include <fstream>

#include "exception/FileNotFoundException.h"
#include "Utils.h"

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>

namespace bson_builder = bsoncxx::builder::basic;

const std::hash<std::string> MongoDatabase::ID_HASHER {};

const std::string MongoDatabase::CONNECTION_STRING_PATH = "./resources/mongodb_connection_string.txt";
std::string MongoDatabase::connectionString = "";

MongoDatabase &MongoDatabase::getInstance()
{
    static MongoDatabase instance;
    return instance;
}

MongoDatabase::MongoDatabase() = default;

bool MongoDatabase::open()
{
    try
    {
        setConnectionString();
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

void MongoDatabase::setConnectionString()
{
    if (!connectionString.empty())
        return;

    connectionString = utils::readFileString(CONNECTION_STRING_PATH);
}

void MongoDatabase::setupMongoConnection()
{
    // Mostly copied from the official docs:

    const auto uri = mongocxx::uri {connectionString};

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
int MongoDatabase::objIdToNumeric(const bsoncxx::oid& id)
{
    return ID_HASHER(id.to_string());
}

mongocxx::collection MongoDatabase::usersCollection() const
{
    return this->m_db.collection("users");
}

mongocxx::collection MongoDatabase::questionsCollection() const
{
    return this->m_db.collection("questions");
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

    return objIdToNumeric(result.value().view()["_id"].get_oid().value);
}

unsigned int MongoDatabase::addNewUser(const std::string &username, const std::string &password,
                                       const std::string &email, const std::string &phone,
                                       const std::string &birthdate,
                                       const std::optional<std::string> &address) const
{
    // Because Magshimim requires no double usernames
    if (doesUserExist(username))
        throw std::runtime_error("User already exists (UNIQUE)");

    bson_builder::document userBuilder {};

    userBuilder.append(
        bson_builder::kvp("username", username),
        bson_builder::kvp("password", password),
        bson_builder::kvp("email", email),
        bson_builder::kvp("phone", phone),
        bson_builder::kvp("birthdate", birthdate)
    );

    if (address.has_value())
    {
        userBuilder.append(
            bson_builder::kvp("address", address.value())
        );
    }

    const auto result = usersCollection().insert_one(userBuilder.extract());


    //TODO: Remove inserting a numId when general ID is converted to oid

    // Add the numeric ID
    const auto filter = bson_builder::make_document(
        bson_builder::kvp("_id", result->inserted_id())
    );

    const auto updateReq = bson_builder::make_document(
        bson_builder::kvp(
            "$set",
            bson_builder::make_document(
                bson_builder::kvp("numId", objIdToNumeric(result->inserted_id().get_oid().value))
            )
        )
    );

    usersCollection().update_one(filter.view(), updateReq.view());


    return objIdToNumeric(result.value().inserted_id().get_oid().value);
}

int MongoDatabase::queryQuestionsCount() const
{
    return questionsCollection().count_documents({});
}

std::list<Question> MongoDatabase::queryQuestions(const int amount) const
{
    auto questionsCursor = questionsCollection().aggregate(mongocxx::pipeline().sample(amount));

    std::list<Question> results;
    for (const auto& questionObj : questionsCursor)
    {
        const std::string prompt = std::string(questionObj["prompt"].get_string().value);
        std::vector<std::string> answers;

        auto answersArr = questionObj["answers"].get_array().value;

        for (const auto &answer : answersArr)
        {
            answers.push_back(std::string(answer.get_string().value));
        }

        results.emplace_back(prompt, answers);
    }

    return results;
}

void MongoDatabase::addQuestions(const std::vector<Question> questions,
                                 const std::optional<std::string> &authorName) const
{
    std::vector<bsoncxx::document::value> questionObjs;
    questionObjs.reserve(questions.size());

    for (const auto& question : questions)
    {
        bson_builder::document questionBuilder {};

        bsoncxx::builder::basic::array answersArray;
        for (const auto& answer : question.answers)
        {
            answersArray.append(answer);
        }

        questionBuilder.append(
            bsoncxx::builder::basic::kvp("prompt", question.prompt),
            bsoncxx::builder::basic::kvp("answers", answersArray)
        );

        if (authorName.has_value())
        {
            questionBuilder.append(
                bson_builder::kvp("authorName", authorName.value())
            );
        }

        questionObjs.emplace_back(questionBuilder.extract());
    }

    questionsCollection().insert_many(questionObjs);
}

void MongoDatabase::addToStats(const std::string &username, int time, int answers, int correctAnswers,
                               int points, int games) const
{
    const auto statsDoc = bson_builder::make_document(
        bson_builder::kvp("stats.points", points),
        bson_builder::kvp("stats.gamesPlayed", games),
        bson_builder::kvp("stats.timeOnQuestionsOverall", time),
        bson_builder::kvp("stats.questionsAnswered", answers),
        bson_builder::kvp("stats.questionsAnsweredCorrect", correctAnswers)
    );


    const auto filter = bson_builder::make_document(
        bson_builder::kvp("username", username)
    );

    const auto updateReq = bson_builder::make_document(
        bson_builder::kvp("$inc", statsDoc)
    );

    usersCollection().update_one(filter.view(), updateReq.view());
}

std::map<UserModel, int> MongoDatabase::queryHighScores(int limit) const
{
    mongocxx::options::find findOptions;

    findOptions.sort(bson_builder::make_document(
        bson_builder::kvp("stats.points", -1)  // Descending order
    ));
    findOptions.limit(50);

    // Only include:
    findOptions.projection(bson_builder::make_document(
        bson_builder::kvp("username", 1),
        bson_builder::kvp("stats.points", 1)
    ));

    auto usersCursor = usersCollection().find({}, findOptions);


    std::map<UserModel, int> results;

    for (const auto &user : usersCursor)
    {
        const auto &stats = user["stats"];

        results.emplace(
            std::piecewise_construct,

            std::forward_as_tuple(
                objIdToNumeric(user["_id"].get_oid().value),
                std::string(user["username"].get_string().value)
            ),

            std::forward_as_tuple(
                stats
                    ? stats.get_document().view()["points"].get_int32().value
                    : 0
            )
        );
    }

    return results;
}

std::optional<UserStatistics> MongoDatabase::getUserStatisticsById(const unsigned int id) const
{
    const auto filter = bson_builder::make_document(
        bson_builder::kvp("numId", static_cast<int>(id))
    );

    mongocxx::options::find findOptions;

    findOptions.projection(bson_builder::make_document(
        bson_builder::kvp("stats", 1)
    ));

    const auto userObj = usersCollection().find_one(filter.view(), findOptions);

    if (!userObj.has_value())
        return std::nullopt;


    const auto &statsObj = userObj.value().view()["stats"];
    if (!statsObj || statsObj.type() != bsoncxx::type::k_document)
        return std::nullopt;

    const auto stats = statsObj.get_document().view();

    const int totalTime = stats["timeOnQuestionsOverall"].get_int32();
    const int totalAns = stats["questionsAnswered"].get_int32();

    return UserStatistics(
        stats["points"].get_int32(),
        stats["gamesPlayed"].get_int32(),
        totalAns,
        stats["questionsAnsweredCorrect"].get_int32(),
        totalTime,
        calcAverageAnswerTime(totalTime, totalAns)
    );
}
