#pragma once

#include "RoomData.h"
#include "db/IDatabase.h"
#include "manager/LoggedUser.h"

#include <optional>
#include <vector>

class Game;

class Room
{
public:
    /**
     * Constructs a new Room.
     *
     * Users should be added manually via Room::addUser.
     */
    Room(LoggedUser& admin, const RoomData& data, const IDatabase& database);

    std::optional<Game*> getCurrentGame() const;
    void setCurrentGame(Game& game);

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

    Game* m_currentGame;

    const IDatabase& m_database;
};