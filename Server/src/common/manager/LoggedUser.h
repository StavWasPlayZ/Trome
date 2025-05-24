#pragma once

#include <string>

class Client;

class LoggedUser
{
public:
    LoggedUser(unsigned int id, const std::string& username, Client& client);

    const std::string& getUsername() const;
    unsigned int getId() const;
    bool operator==(const LoggedUser &other) const;

    Client& getClient() const;
    
private:
    const unsigned int m_id;
    std::string m_username;

    Client& m_client;
};
