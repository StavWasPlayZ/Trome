#include "Room.h"

#include "Client.h"
#include "Server.h"

#include "db/IDatabase.h"
#include "handler/IRequestHandler.h"
#include "handler/MenuRequestHandler.h"

unsigned int Room::globalId = 0;

Room::Room(const unsigned int id, LoggedUser &admin, const RoomData &data,
           const IDatabase &database, const RoomStatus status) :
    id(id),
    // REVIEW: Perhaps could be initialized directly.
    // On this condition that Not Found it not set.
    status(status),
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
    return this->id == other.id;
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

const std::vector<LoggedUser *> &Room::getAllUsers() const
{
    return this->m_users;
}

unsigned int Room::getId() const
{
    return this->id;
}

const RoomData &Room::getData() const
{
    return this->m_metadata;
}

void Room::setData(const RoomData &newData)
{
    this->m_metadata = newData;

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
    return this->status;
}

void Room::setStatus(const RoomStatus status)
{
    this->status = status;
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
