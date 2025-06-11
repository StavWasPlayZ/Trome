#pragma once

#include <string>

//TODO: Make LoggedUser extend UserModel
struct UserModel
{
    UserModel(unsigned int id, const std::string& username);

    unsigned int id;
    std::string username;

    //TODO: Identifiable parent.
    bool operator==(const UserModel &other) const;
    bool operator<(const UserModel& other) const;
};
