#pragma once

#include "infrastructure/Question.h"
#include "infrastructure/UserStatistics.h"
#include "infrastructure/model/UserModel.h"

#include <ctre.hpp>
#include <list>
#include <map>
#include <optional>

class IDatabase
{
public:
    IDatabase(const IDatabase&) = delete;
    void operator=(const IDatabase&) = delete;

	virtual ~IDatabase();

	/**
	 * Open the database.
	 * 
	 * Returns: Whether the operation succeed
	 */
	virtual bool open() = 0;
	/**
	 * Closes the database
	 * 
	 * Returns: Whether the operation succeed
	 */
	virtual bool close() = 0;
	/**
	 * Returns: Whether the user exists within the database
	 */
	virtual bool doesUserExist(const std::string& username) const = 0;
	/**
	 * Returns: The ID of the user matching the provided credentials; -1 otherwise.
	 */
	virtual long queryIdOfUser(const std::string& username, const std::string& password) const = 0;
	/**
	 * Adds a new user.
	 * 
	 * Returns: Its new ID
	 */
	virtual long addNewUser(
		const std::string& username,
		const std::string& password,
		const std::string& email,
		const std::string& phone,
		const std::string& birthdate,
		const std::optional<std::string>& address
	) const = 0;


    virtual int queryQuestionsCount() const = 0;
    virtual std::list<Question> queryQuestions(int amount) const = 0;

    /**
     * Add multiple questions to the DB.
     *
     * If no author name is provided, assuming server-instantiated.
     */
    virtual void addQuestions(std::vector<Question> questions, const std::optional<std::string>& authorName) const = 0;


    // Statistics:
    virtual void addToStats(const std::string &username, int time, int answers, int correctAnswers, int points,
                            int games = 1) const = 0;

    virtual std::optional<UserStatistics> getUserStatisticsById(long id) const = 0;

	virtual std::map<UserModel, int> queryHighScores(int limit = 20) const = 0;

protected:
    IDatabase() = default;

    static float calcAverageAnswerTime(int totalTime, int totalAns);
};
