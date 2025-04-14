#pragma once

#include <iostream>

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
	 * Returns: Whether the user exists within the databse
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
	virtual unsigned int addNewUser(const std::string& username, const std::string& password, const std::string& email) const = 0;	
};
