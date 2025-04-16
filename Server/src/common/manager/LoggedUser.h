#pragma once

#include <string>
#include "infrastructure/Client.h"

class LoggedUser
{
public:
    LoggedUser(unsigned int id, const std::string& username, const Client* client);
    const std::string& getUsername() const;
    unsigned int getId() const;
    bool operator==(const LoggedUser &other) const;
    
private:
    const unsigned int m_id;
    std::string m_username;

    const Client* const m_client;
};
