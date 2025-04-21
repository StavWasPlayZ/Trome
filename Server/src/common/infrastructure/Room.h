#pragma once

/**
* DO NOT INCLUDE #include "infrastructure/RoomData.h".
*
* this file includes manager/LoggedUser.h
* ...that includes infrastructure/Client.h
* ...that includes handler/IRequestHandler.h
* ...that includes infrastructure/request/RequestInfo.h
* ...which has enum class RequestCode : unsigned char;
* ...which includes back "infrastructure/RoomData.h".
*
* Forward declaration may be possible here for that matter, though untested.
*/

#include <string>
#include <vector>
#include "manager/LoggedUser.h"

class Room
{
public:
    /**
     * Constructs a new Room.
     *
     * Users should be added manually via Room::addUser.
     */
    Room(LoggedUser& admin, const RoomData& data);

    void addUser(LoggedUser& user);
    void removeUser(const LoggedUser& user);
    std::vector<LoggedUser*> getAllUsers() const;

    RoomData& getData();

    LoggedUser& getAdmin() const;
    void setAdmin(LoggedUser& admin);

private:
    // Made a pointer such that if we'd like to change it in the future
    LoggedUser* m_admin;

    RoomData m_metadata;
    std::vector<LoggedUser*> m_users;
};