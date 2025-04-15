#pragma once

#include <optional>
#include <ctre.hpp>

class IDatabase
{
public:
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
	virtual unsigned int getIdOfUser(const std::string& username, const std::string& password) const = 0;
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

protected:
    // Regexes.

    //NOTE: Made the password length be at LEAST *8* characters, not *exactly*.
	inline static auto passMatcher = ctre::match<
        "^(?=.{8,})(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[!@#$%^&*])(?:.*)$"
    >;

    //NOTE: This captures an email address generally case-sensitive to lowercase only.
    // The client should only provide the server emails with lowercase emails.
    // Emails are non-case-sensitive anyway.
    // Also, does not capture for periods and alike. Same reason - they are ignored in emails.
    // This helps email uniqueness within the database.
    //
    // Also, TLD has at least 2 characters, the rest idc.
	inline static auto emailMatcher = ctre::match<
        "^[a-z\\d]+@([a-z\\d]+\\.)+[a-z\\d]{2,}$"
    >;

    //NOTE: Made Street & City inclusive of spaces too.
	inline static auto addressMatcher = ctre::match<
        "^[A-Za-z ]+, \\d+, [A-Za-z ]+$"
    >;

    //NOTE: This is not how phone numbers work in general, but sure.
    // As requested, either 3 digits or 2 digits prefix starting with 0.
    // After the prefix will always be 7 digits.
    // The prefixes were not specified to be bound.
	inline static auto phoneMatcher = ctre::match<
        "^0.{8,9}$"
    >;


	static void validateSignupInfo(
		const std::string& email,
		const std::string& phone,
		const std::string& birthdate,
		const std::optional<std::string>& address
	);
};
