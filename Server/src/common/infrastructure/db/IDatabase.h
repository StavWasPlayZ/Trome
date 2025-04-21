#pragma once

#include "infrastructure/Question.h"

#include <ctre.hpp>
#include <list>
#include <optional>
#include <unordered_map>

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
	virtual unsigned int queryIdOfUser(const std::string& username, const std::string& password) const = 0;
	/**
	 * Adds a new user.
	 * 
	 * Returns: Its new ID
	 */
	virtual unsigned int addNewUser(
		const std::string& username,
		const std::string& password,
		const std::string& email,
		const std::string& phone,
		const std::string& birthdate,
		const std::optional<std::string>& address
	) const = 0;


    virtual int queryQuestionsCount() const = 0;
    virtual std::list<Question> queryQuestions(int amount) const = 0;
    virtual void addQuestions(std::vector<Question> questions) const = 0;


    // Adding to statistics:

	virtual void addTime(const std::string &username, int time) = 0;
	virtual void addTotalAns(const std::string &username, int ans = 1) = 0;
	virtual void addCorrectAns(const std::string &username, int ans = 1) = 0;
	virtual void addGamesPlayed(const std::string &username, int games = 1) = 0;
	virtual void addPoints(const std::string &username, int points) = 0;

    // Retrieving statistics:

	virtual int queryTime(const std::string &username) const = 0;
	virtual int queryTotalAns(const std::string &username) const = 0;
	virtual int queryCorrectAns(const std::string &username) const = 0;
	virtual int queryGamesPlayed(const std::string &username) const = 0;
	virtual int queryPoints(const std::string &username) const = 0;
    virtual float queryPlayerAverageAnsTime(const std::string &username) const = 0;

	virtual std::unordered_map<std::string, int> queryHighScores(int limit = 20) const = 0;

protected:
    IDatabase() = default;

	
	/**
     * Returns: The ID of the user connected user; -1 otherwise.
     */
    virtual unsigned int queryIdOfUser(const std::string &username) const = 0;

	/**
	* Adds `n` to the specified column for the given user.
	*
	* Said column must be numerable.
	*/
    virtual void addToColumn(const std::string &username, const std::string &column, int n,
                             const std::string &table) = 0;

    // Regexes.

    //NOTE: Made the password length be at LEAST 8 characters, not *exactly*.
	static constexpr auto passMatcher = ctre::match<
        "^(?=.{8,})(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[!@#$%^&*])(?:.*)$"
    >;

    //NOTE: This captures a lowercase-only email address.
    // The client should only provide the server emails in lowercase.
    // Emails are non-case-sensitive anyway.
    // Also, does not capture for periods and alike. Same reason - they are ignored in emails.
    // This helps email uniqueness within the database.
    //
    // Also, TLD has at least 2 characters, the rest idc.
	static constexpr auto emailMatcher = ctre::match<
        "^[a-z\\d]+@([a-z\\d]+\\.)+[a-z\\d]{2,}$"
    >;

    //NOTE: Made Street & City inclusive of spaces too.
	static constexpr auto addressMatcher = ctre::match<
        "^[A-Za-z ]+, \\d+, [A-Za-z ]+$"
    >;

    //NOTE: This is not how phone numbers work in general, but sure.
    // As requested, either 3 digits or 2 digits prefix starting with 0.
    // After the prefix will always be 7 digits.
    // The prefixes were not specified to be bound.
	static constexpr auto phoneMatcher = ctre::match<
        "^0.{8,9}$"
    >;

	//NOTE: Regexes can't actually verify whether dates are real.
    // Even if I bound the numbers into maximum values, it'd be wholly pointless
    // because they may be invalid as well, depending on the actual calendar.
    // So, shall be kept simple.
	static constexpr auto dateMatcher = ctre::match<
		"^\\d{2}\\/\\d{2}\\/\\d{4}$"
	>;


	static void validateSignupInfo(
		const std::string& password,
		const std::string& email,
		const std::string& phone,
		const std::string& birthdate,
		const std::optional<std::string>& address
	);
};
