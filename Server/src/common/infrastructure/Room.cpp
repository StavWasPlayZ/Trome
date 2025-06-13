#include "Room.h"

#include "Client.h"
#include "Server.h"

#include "db/IDatabase.h"
#include "handler/IRequestHandler.h"
#include "handler/MenuRequestHandler.h"

unsigned int Room::globalId = 0;

Room::Room(const unsigned int id, const RoomType roomType, LoggedUser &admin, const RoomData &data,
           const IDatabase &database, const RoomStatus status) :
    m_id(id),
    // REVIEW: Perhaps could be initialized directly.
    // On this condition that Not Found does not exist.
    m_status(status),
    m_roomType(roomType),
    m_admin(&admin),
    m_metadata(data),
    m_currentGame(nullptr),
    m_database(database),
    m_handlerFactory(Server::getInstance().getRequestHandlerFactory())
{
    // Add the admin to the room
    addUser(admin);
}

Room::~Room()
{
    // This assumes that the room was deleted as a result of the admin deleting it.
    // It thus does not alter the admin's handler.
    //
    // Hence, we will simply simulate the admin leaving the room, thus gracefully closing
    // it and kicking all others:
    removeUser(getAdmin());
}

bool Room::operator==(const Room &other) const
{
    return this->m_id == other.m_id;
}

unsigned int Room::generateId()
{
    return globalId++;
}

std::optional<Game *> Room::getCurrentGame() const
{
    if (this->m_currentGame == nullptr)
        return std::nullopt;

    return this->m_currentGame;
}

Game &Room::createNewGame(GameManager &gameManager)
{
    if (getCurrentGame().has_value())
        throw std::runtime_error("Game already in process");

    this->m_currentGame = &gameManager.createGame(*this);

    return *this->m_currentGame;
}

void Room::unsetCurrentGame()
{
    this->m_currentGame = nullptr;
}

void Room::addUser(LoggedUser &user)
{
    this->m_users.push_back(&user);
    user.setCurrentRoom(*this);

    IRequestHandler::dispatchNotification(
        PlayerJoinedRoomNotification(user),
        getAllUsers(),
        &user
    );
}

void Room::removeUser(LoggedUser &user)
{
    const auto it = std::ranges::find(this->m_users, &user);

    if (it == m_users.end())
        return;

    m_users.erase(it);
    user.removeFromRoom();


    if (getCurrentGame().has_value())
    {
        getCurrentGame().value()->handlePlayerFinished(user);
    }


    if (user == getAdmin())
    {
        handleAdminLeft(user);
    }
    else
    {
        handleGuestLeft(user);
    }
}

void Room::kickUser(LoggedUser &user)
{
    const auto it = std::ranges::find(this->m_users, &user);

    if (it == m_users.end())
        return;

    m_users.erase(it);
    user.removeFromRoom();


    IRequestHandler::setRequestHandlers(
        [this](const LoggedUser *const)
        {
            return new MenuRequestHandler(m_handlerFactory);
        },

        {&user} // makes a std::vector<LoggedUser*> with the kicked player
    );

    IRequestHandler::dispatchNotification(
        PlayerKickedNotification(user.getId()),
        getAllUsers(), 
        &getAdmin()
    );
}

const std::vector<LoggedUser *> &Room::getAllUsers() const
{
    return this->m_users;
}

unsigned int Room::getId() const
{
    return this->m_id;
}

const RoomData &Room::getData() const
{
    return this->m_metadata;
}

void Room::setData(const RoomData &newData)
{
    this->m_metadata = newData;

    if (m_roomType == RoomType::HEAD_TO_HEAD)
    {
        // Disallow any max players count besides 2
        this->m_metadata.maxPlayers = 2;
    }

    // Dispatch updates to all users.
    IRequestHandler::dispatchNotification(
        RoomDataUpdatedNotification(newData),
        getAllUsers(),
        // Assuming the admin initiated the call
        &getAdmin()
    );
}

LoggedUser &Room::getAdmin() const
{
    return *m_admin;
}

void Room::setAdmin(LoggedUser &admin)
{
    this->m_admin = &admin;
}

RoomStatus Room::getStatus() const
{
    return this->m_status;
}

RoomType Room::getType() const
{
    return this->m_roomType;
}

void Room::setStatus(const RoomStatus status)
{
    this->m_status = status;
}

void Room::handleGuestLeft(const LoggedUser &guest) const
{
    IRequestHandler::dispatchNotification(
        PlayerLeftRoomNotification(guest.getId()),
        getAllUsers()
    );
}

void Room::handleAdminLeft(const LoggedUser &) const
{
    // Also disconnect all other players
    for (LoggedUser* player : getAllUsers())
    {
        player->getClient().setRequestHandlerSafe(new MenuRequestHandler(m_handlerFactory));
        player->removeFromRoom();
    }

    if (getCurrentGame().has_value())
    {
        getCurrentGame().value()->endGame();
    }

    IRequestHandler::dispatchNotification(
        RoomClosedNotification(),
        getAllUsers()
    );
}
