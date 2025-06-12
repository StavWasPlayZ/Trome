#pragma once

#include "RoomData.h"
#include "db/IDatabase.h"
#include "manager/LoggedUser.h"

#include <optional>
#include <vector>

enum class RoomType;
class GameManager;
class RequestHandlerFactory;
enum class RoomStatus : unsigned int;
class Game;

class Room
{
public:
    /**
     * Constructs a new Room.
     *
     * Users should be added manually via Room::addUser.
     */
    Room(unsigned int id, RoomType roomType, LoggedUser& admin, const RoomData& data, const IDatabase& database,
        RoomStatus status);
    ~Room();

    //TODO: Make part of an identifiable (LoggedUser is too)
    bool operator==(const Room &other) const;

    static unsigned int generateId();


    std::optional<Game*> getCurrentGame() const;
    Game &createNewGame(GameManager& gameManager);
    void unsetCurrentGame();

    void addUser(LoggedUser& user);
    void removeUser(LoggedUser& user);
    const std::vector<LoggedUser*>& getAllUsers() const;


    unsigned int getId() const;

    const RoomData& getData() const;
    RoomType getType() const;
    void setData(const RoomData& newData);

    LoggedUser& getAdmin() const;
    void setAdmin(LoggedUser& admin);

    RoomStatus getStatus() const;
    void setStatus(RoomStatus status);

private:
    const unsigned int m_id;
    RoomStatus m_status;
    RoomType m_roomType;

    // Made a pointer such that if we'd like to change it in the future
    LoggedUser* m_admin;

    void handleGuestLeft(const LoggedUser &guest) const;
    void handleAdminLeft(const LoggedUser &admin) const;

    RoomData m_metadata;
    std::vector<LoggedUser*> m_users;

    Game* m_currentGame;

    const IDatabase& m_database;
    const RequestHandlerFactory& m_handlerFactory;

    /**
     * Used for counting how many instances of Room exists,
     * generating a unique ID for each.
     */
    static unsigned int globalId;
};


enum class RoomType
{
    TRIVIA_RUSH,
    HEAD_TO_HEAD
};

enum class RoomStatus : unsigned int
{
    WAITING = 0,
    PLAYING = 1,
    //TODO: Remove
    NOT_FOUND = 2
};
