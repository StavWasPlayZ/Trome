#include "LoggedUser.h"

LoggedUser::LoggedUser(const unsigned int id, const std::string &username, const Client *const client) :
    m_id(id),
    m_username(username),
    m_client(client)
{}

const std::string &LoggedUser::getUsername() const
{
    return this->m_username;
}

unsigned int LoggedUser::getId() const
{
    return this->m_id;
}
