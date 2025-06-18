#pragma once

#include <unordered_map>

#include "LoggedUser.h"

#include "../handler/codec/c2s/request/Request.h"
#include "../handler/codec/s2c/response/Response.h"
#include "infrastructure/db/IDatabase.h"

class LoginManager
{
public:
	explicit LoginManager(const IDatabase& database);

	/**
	 * Signs up a new user, then registers them as logged in.
	 *
	 * NOTE: THE RETURNED RESOURCE MUST BE FREED
	 */
	ProtocolResponse* signup(const RequestInfo &info, const SignupRequest &request);

	/**
	 * Registers the provided user as logged in, provided their credentials match.
	 *
	 * NOTE: THE RETURNED RESOURCE MUST BE FREED
	 */
	ProtocolResponse* login(const RequestInfo &info, const LoginRequest &request);

	/**
	 * Unregisters the user as being signed in.
	 *
	 * Returns: True if the user has successfully logged out, or false if they
	 * weren't logged in to begin with.
	 */
	bool logout(const Client &client);

	bool isLoggedIn(const Client &client) const;
    LoggedUser& getUserByClient(const Client& client) const;
    LoggedUser& getUserByUsername(const std::string& username);
    LoggedUser &getUserById(unsigned int id) const;

    bool isLoggedIn(unsigned int id) const;

private:
	const IDatabase& m_database;

	/**
	 * Maps a username to a logged in user.
	 * Mapping as such because the keys are based, for some reason, on usernames only.
	 * 
	 * TODO: Ask if it may be done with IDs.
	 *
	 * (...Or not because we do it below anyways.)
	 */
	std::unordered_map<std::string, LoggedUser> m_loggedUsers;

    std::unordered_map<unsigned int, LoggedUser*> m_loggedUsersById;
    std::unordered_map<const Client*, LoggedUser*> m_clientToLoggedUser;


    // Regexes.
    static void validateSignupInfo(
        const std::string& password,
        const std::string& email,
        const std::string& phone,
        const std::string& birthdate,
        const std::optional<std::string>& address
    );

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
};