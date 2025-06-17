#pragma once

#include "IDatabase.h"

#include <functional>
#include <list>
#include <map>
#include <string>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

class MongoDatabase : public IDatabase
{
public:
    MongoDatabase(const MongoDatabase &) = delete;
    void operator=(const MongoDatabase &) = delete;

    static MongoDatabase &getInstance();

    bool open() override;
    bool close() override;
    bool doesUserExist(const std::string &username) const override;
    unsigned int queryIdOfUser(const std::string &username, const std::string &password) const override;

    unsigned int addNewUser(const std::string &username, const std::string &password, const std::string &email,
                            const std::string &phone, const std::string &birthdate,
                            const std::optional<std::string> &address) const override;

    int queryQuestionsCount() const override;
    std::list<Question> queryQuestions(int amount) const override;

    void addQuestions(std::vector<Question> questions, const std::optional<std::string> &authorName) const override;

    void addToStats(const std::string &username, int time, int answers, int correctAnswers, int points,
                    int games = 1) const override;

    std::map<UserModel, int> queryHighScores(int limit = 20) const override;

    std::optional<UserStatistics> getUserStatisticsById(unsigned int id) const override;

private:
    static const std::string CONNECTION_STRING_PATH;
    static std::string connectionString;
    static bool connectionStringLoaded;

    static const std::hash<std::string> ID_HASHER;

    const mongocxx::instance inst;

    MongoDatabase();

    void setupMongoConnection();
    void setupDbConnections();
    void setConnectionString();

    static int objIdToNumeric(const bsoncxx::oid& id);

    mongocxx::collection usersCollection() const;
    mongocxx::collection questionsCollection() const;

    mongocxx::client m_mongoClient;

    mongocxx::database m_db;
};
