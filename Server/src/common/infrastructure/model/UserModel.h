#pragma once

#include <string>

//TODO: Make LoggedUser extend UserModel
struct UserModel
{
    UserModel(long id, const std::string& username);

    long id;
    std::string username;

    //TODO: Identifiable parent.
    bool operator==(const UserModel &other) const;
    bool operator<(const UserModel& other) const;
};
