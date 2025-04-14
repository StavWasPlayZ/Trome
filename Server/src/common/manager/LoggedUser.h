#pragma once

#include <string>

class LoggedUser
{
public:
    LoggedUser(unsigned int id, const std::string& username);
    
    const std::string& getUsername() const;
    unsigned int getId() const;
    
private:
    const unsigned int m_id;
    std::string m_username;

    //TODO: Add Client<T>*.
};
