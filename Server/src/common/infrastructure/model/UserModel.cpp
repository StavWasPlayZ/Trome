#include "UserModel.h"

UserModel::UserModel(const long id, const std::string &username) :
    id(id),
    username(username)
{}

bool UserModel::operator==(const UserModel &other) const
{
    return this->id == other.id;
}

bool UserModel::operator<(const UserModel &other) const
{
    return this->id < other.id;
}
