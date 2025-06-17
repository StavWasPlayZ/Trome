#include "MongoDatabase.h"

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
}

bool MongoDatabase::close()
{
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
